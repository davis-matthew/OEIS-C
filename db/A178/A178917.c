/*
 * A178917  Pairs prime(k), prime(k+1) such that last digit of prime(k) is 9
 *          and last digit of prime(k+1) is 7.
 * https://oeis.org/A178917
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) is the n‑th element of the sequence (p, next_prime(p))
 *            where p ends with 9 and the next prime ends with 7. The sequence
 *            alternates between p and its successor
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* 64‑bit deterministic Miller–Rabin primality test */
static uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__int128)a * b % mod);
}

static uint64_t pow_mod64(uint64_t a, uint64_t e, uint64_t mod)
{
    uint64_t r = 1 % mod;
    while (e)
    {
        if (e & 1) r = mul_mod64(r, a, mod);
        a = mul_mod64(a, a, mod);
        e >>= 1;
    }
    return r;
}

static bool is_prime64(uint64_t n)
{
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;

    /* Deterministic bases for 64‑bit integers */
    const uint64_t bases[] = {2, 3, 5, 7, 11, 13, 17};
    uint64_t d = n - 1;
    int s = 0;
    while ((d & 1) == 0) { d >>= 1; ++s; }

    for (size_t i = 0; i < sizeof(bases) / sizeof(bases[0]); ++i)
    {
        uint64_t a = bases[i];
        if (a % n == 0) continue;
        uint64_t x = pow_mod64(a, d, n);
        if (x == 1 || x == n - 1) continue;
        bool cont = false;
        for (int r = 1; r < s; ++r)
        {
            x = mul_mod64(x, x, n);
            if (x == n - 1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* ---------- cache for the generated sequence ---------- */
static seq_t *seq_cache = NULL;
static size_t seq_len = 0, seq_cap = 0;
static uint64_t last_cand = 2;
static uint64_t prev_prime = 0;

static void ensure_seq_cache(size_t needed)
{
    if (needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap < needed) new_cap <<= 1;
    if (seq_cache == NULL)
        seq_cache = (seq_t *)realloc(NULL, new_cap * sizeof(seq_t));
    else
        seq_cache = (seq_t *)realloc(seq_cache,
                                     new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* candidate generator: returns next odd number to test
   (or 2 on the first call) */
static uint64_t next_candidate(void)
{
    uint64_t cand;
    if (last_cand <= 2) {
        cand = 2;
        last_cand = 3;
    } else {
        cand = last_cand;
        last_cand += 2;
    }
    return cand;
}

/* ---------- the sequence function ---------- */
extern seq_t a(idx_t n)  /* external linkage mandatory */
{
    if (n == 0) return (seq_t)0;  /* guard, offset is 1 */

    /* generate until the cache holds at least n elements */
    while (seq_len < (size_t)n)
    {
        uint64_t cand = next_candidate();
        if (!is_prime64(cand)) continue;

        if (prev_prime != 0)
        {
            if (prev_prime % 10 == 9 && cand % 10 == 7)
            {
                /* store the pair */
                ensure_seq_cache(seq_len + 2);
                seq_cache[seq_len++] = (seq_t)prev_prime;
                seq_cache[seq_len++] = (seq_t)cand;
            }
        }
        prev_prime = cand;
    }
    return seq_cache[n - 1];
}
