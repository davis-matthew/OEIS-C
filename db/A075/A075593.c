/*
 * A075593  a(1) = 1, a(2n) = prime, a(2n+1) = composite and sum of two
 *          successive terms is alternately composite or prime.
 * https://oeis.org/A075593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A075593: a(1)=1.  For even i, a(i) is the smallest unused prime
   such that a(i−1)+a(i) is composite.
   For odd i>1, a(i) is the smallest unused composite such that
   a(i−1)+a(i) is prime.
   The sequence is generated greedily.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* ---------- prime test (deterministic for 64‑bit) ---------- */
static uint64_t modpow64(uint64_t a, uint64_t d, uint64_t mod)
{
    uint64_t r = 1;
    while (d)
    {
        if (d & 1ULL)
            r = (unsigned __int128)r * a % mod;
        a = (unsigned __int128)a * a % mod;
        d >>= 1;
    }
    return r;
}

static bool is_prime_uint64(uint64_t n)
{
    if (n < 2)              return false;
    if (n % 2ULL == 0ULL)   return n == 2ULL;
    static const uint64_t bases[] = {2ULL,3ULL,5ULL,7ULL,11ULL,13ULL,17ULL};
    uint64_t d = n - 1, s = 0;
    while ((d & 1ULL) == 0ULL) { d >>= 1; ++s; }
    for (size_t i = 0; i < sizeof(bases)/sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i];
        if (a >= n) continue;
        uint64_t x = modpow64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool composite = true;
        for (uint64_t r = 1; r < s; ++r)
        {
            x = (unsigned __int128)x * x % n;
            if (x == n - 1) { composite = false; break; }
        }
        if (composite) return false;
    }
    return true;
}

/* ---------- global storage for the sequence ---------- */
static seq_t *seq = NULL;     /* 0‑based array, seq[0] = a(1) */
static idx_t seq_len = 0;
static size_t seq_cap = 0;

static void ensure_seq_capacity(size_t need)
{
    if (need <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    seq = tmp;
    seq_cap = new_cap;
}

/* check whether value has already appeared in the sequence */
static bool used(seq_t v)
{
    for (idx_t i = 0; i < seq_len; ++i)
        if (seq[i] == v) return true;
    return false;
}

/* find the next term according to the rules */
static seq_t find_next(idx_t prev_idx)
{
    seq_t prev = seq[prev_idx];
    bool expect_prime = (prev_idx % 2 == 0);   /* even index -> need prime */
    seq_t cand = expect_prime ? 2 : 4;

    for (;; ++cand)
    {
        if (used(cand)) continue;

        /* check type */
        bool cand_is_prime;
        if ((sizeof(seq_t) <= 8) && (cand >> 63))
        {   /* candidate too large for 64‑bit, can't be prime /\ composite */
            // continue; // but this case should never happen for n ≤ 10000
            cand_is_prime = false;
        } else {
            cand_is_prime = is_prime_uint64((uint64_t)cand);
        }

        if (expect_prime && !cand_is_prime) continue;
        if (!expect_prime && cand_is_prime) continue;
        if (!expect_prime && cand <= 3) continue;   /* composites start at 4 */

        /* check sum condition */
        seq_t sum = prev + cand;
        bool sum_is_prime = (sizeof(seq_t) <= 8) && (sum >> 63) ? false :
                            is_prime_uint64((uint64_t)sum);

        if (expect_prime)
        {   /* sum should be composite */
            if (sum_is_prime) continue;
        }
        else
        {   /* sum should be prime */
            if (!sum_is_prime) continue;
        }

        return cand;
    }
}

/* ---------- public function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined, but avoid crash */

    if (seq_len < n)
    {
        ensure_seq_capacity(n);
        if (seq_len == 0)
        {
            seq[0] = 1;
            seq_len = 1;
        }
        for (idx_t i = seq_len; i < n; ++i)
        {
            seq[i] = find_next(i - 1);
            ++seq_len;
        }
    }
    return seq[n - 1];
}
