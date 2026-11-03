/*
 * A073661  Rearrangement of even numbers such that a(k) + a(k+1) + 1 is a
 *          prime for all k.
 * https://oeis.org/A073661
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 62, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 65 terms through n = 65 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A greedy construction of OEIS A073661:
   a(1) = 2
   a(n) is the next unused even number e such that
   a(n-1) + e + 1 is prime.  The sequence is built
   inductively and memoised for random access.
   The implementation uses a dynamic array of the
   terms and a dynamic bitmap of evens that have
   already been used.
*/

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* static storage for the sequence and used flags */
static seq_t *seq = NULL;               /* terms of the sequence   */
static size_t seq_len = 0;              /* number of terms stored */
static size_t seq_cap = 0;              /* capacity of seq array  */

static bool *used = NULL;               /* bitmap of used even numbers */
static size_t used_cap = 0;             /* number of even indices tracked */

/* ensure that the seq array can hold at least 'need' elements */
static void ensure_seq(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 256;
    while (new_cap < need) new_cap *= 2;
    seq_t *ptr = mem_resize(seq_t, seq, new_cap);
    seq = ptr;
    seq_cap = new_cap;
}

/* ensure that the used bitmap can index even number 'idx' */
static void ensure_used(size_t idx)
{
    if (used_cap > idx) return;
    size_t new_cap = used_cap ? used_cap * 2 : 256;
    while (new_cap <= idx) new_cap *= 2;
    bool *ptr = (bool *)realloc(used, new_cap * sizeof(bool));
    /* zero the newly added slots */
    memset(ptr + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = ptr;
    used_cap = new_cap;
}

/* public interface required by the test harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* undefined for n < 1, but guard anyway */

    /* build the sequence up to index n */
    while (seq_len < (size_t)n) {

        /* initialise first term */
        if (seq_len == 0) {
            seq_t first = 2;
            ensure_seq(1);
            seq[0] = first;
            seq_len = 1;
            size_t id = (size_t)(first >> 1);   /* 2 -> index 1 */
            ensure_used(id);
            used[id] = true;
            continue;
        }

        /* find next term greedily */
        seq_t prev   = seq[seq_len - 1];
        seq_t chosen = 0;
        size_t chosen_idx = 0;

        for (seq_t cand = 2; ; cand += 2) {
            size_t id = (size_t)(cand >> 1);
            ensure_used(id);
            if (used[id]) continue;      /* already placed */

            seq_t s = prev + cand + 1;   /* check primality of sum+1 */
            if (is_prime_big(s)) {
                chosen = cand;
                chosen_idx = id;
                break;
            }
        }

        /* store the selected term */
        ensure_seq(seq_len + 1);
        seq[seq_len] = chosen;
        seq_len++;
        used[chosen_idx] = true;
    }

    return seq[n - 1];
}
