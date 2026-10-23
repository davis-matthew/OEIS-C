/*
 * A162842  a(1)=1, a(2)=2; for n > 2, a(n) is the smallest m that has not yet
 *          appeared and is not divisible by any pairwise sum of a(1)..a(n-1).
 * https://oeis.org/A162842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A162842
   a(1) = 1, a(2) = 2;
   For n > 2, a(n) is the smallest integer m not yet in the sequence
   and not divisible by any pairwise sum of a(1)..a(n-1).
   This implementation keeps the sequence in a static array and
   stores all pairwise sums in a dynamic bitset.  For a candidate
   m, it is forbidden if m itself is a sum or if any divisor of m
   is a known sum.  The algorithm runs in about linear time for
   n up to ~10000. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* ---------- configuration ---------- */
#define MAX_TERMS          200000
#define INIT_BITSET_BYTES  16

/* ---------- static data ---------- */
static seq_t seq[MAX_TERMS];
static size_t seq_len = 0;

/* bitset of pairwise sums: bit i set if i is a sum of two terms. */
static uint8_t *sum_bits = NULL;
static size_t sum_bitset_bytes = 0;

/* ---------- helper: grow bitset ---------- */
static void ensure_sum_bits_bytes(size_t bytes_needed)
{
    if (bytes_needed <= sum_bitset_bytes) return;
    size_t new_bytes = sum_bitset_bytes > 0 ? sum_bitset_bytes << 1 : INIT_BITSET_BYTES;
    while (new_bytes < bytes_needed) new_bytes <<= 1;
    uint8_t *tmp = (uint8_t *)realloc(sum_bits, new_bytes);
    if (!tmp) abort();
    /* zero the newly allocated part */
    memset(tmp + sum_bitset_bytes, 0, new_bytes - sum_bitset_bytes);
    sum_bits = tmp;
    sum_bitset_bytes = new_bytes;
}

static void set_sum_bit(size_t bit)
{
    size_t byte = bit >> 3;
    size_t bytes_needed = byte + 1;
    ensure_sum_bits_bytes(bytes_needed);
    sum_bits[byte] |= (uint8_t)(1u << (bit & 7));
}

static bool get_sum_bit(size_t bit)
{
    size_t byte = bit >> 3;
    if (byte >= sum_bitset_bytes) return false;
    return (sum_bits[byte] >> (bit & 7)) & 1u;
}

/* ---------- integer square root for 64‑bit ---------- */
static uint64_t isqrt_u64(uint64_t n)
{
    uint64_t low = 0, high = n, mid;
    while (low <= high) {
        mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == n) return mid;
        if (sq < n)   low = mid + 1;
        else          high = mid - 1;
    }
    return high;
}

/* ---------- test whether a candidate is forbidden ---------- */
static bool is_candidate_forbidden(seq_t m)
{
    size_t bit = (size_t)m;
    if (get_sum_bit(bit)) return true;          /* m is a sum itself */

    uint64_t n = (uint64_t)m;                   /* candidate fits in 64‑bit */
    uint64_t sqrt_n = isqrt_u64(n);
    for (uint64_t d = 2; d <= sqrt_n; ++d) {
        if (n % d == 0) {
            size_t bitd = (size_t)d;
            if (get_sum_bit(bitd)) return true;
            uint64_t q = n / d;
            if (q != d) {
                size_t bitq = (size_t)q;
                if (get_sum_bit(bitq)) return true;
            }
        }
    }
    return false;
}

/* ---------- add a new term and record its sums ---------- */
static void add_term(seq_t m)
{
    if (seq_len >= MAX_TERMS) abort();
    for (size_t i = 0; i < seq_len; ++i) {
        seq_t s = seq[i] + m;
        size_t bit = (size_t)s;
        set_sum_bit(bit);
    }
    seq[seq_len++] = m;
}

/* ---------- initialise sequence with first two terms ---------- */
static void init_sequence(void)
{
    add_term((seq_t)1);
    add_term((seq_t)2);
}

/* ---------- generate sequence up to at least index n ---------- */
static void generate_up_to(idx_t n)
{
    while (seq_len < (size_t)n) {
        seq_t candidate = seq[seq_len - 1] + (seq_t)1;
        while (is_candidate_forbidden(candidate))
            candidate = candidate + (seq_t)1;
        add_term(candidate);
    }
}

/* ---------- exported function ---------- */
seq_t a(idx_t n)
{
    if (seq_len == 0) init_sequence();
    if ((size_t)n > seq_len) generate_up_to(n);
    return seq[(size_t)n - 1];
}
