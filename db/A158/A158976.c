/*
 * A158976  a(n) = sum of numbers k <= n such that not all proper divisors of
 *          k are divisors of n.
 * https://oeis.org/A158976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of all k <= n such that not all proper divisors of k
 *            divide n
 * Range:     u8 n <= 28, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Global cache of smallest prime factors */
static uint32_t *spf = NULL;
static idx_t spf_size = 0;

/* Build smallest prime factor array up to limit (inclusive) */
static void build_spf(idx_t limit)
{
    size_t sz = (size_t)(limit + 1);
    uint32_t *new_spf = (uint32_t *)malloc(sz * sizeof(uint32_t));
    if (!new_spf) return;                       /* allocation failure – silent */
    for (size_t i = 0; i < sz; ++i) new_spf[i] = 0;
    for (size_t i = 2; i < sz; ++i)
    {
        if (!new_spf[i])
        {
            new_spf[i] = (uint32_t)i;
            if ((uint64_t)i * (uint64_t)i < sz)
            {
                for (size_t j = i * i; j < sz; j += i)
                    if (!new_spf[j]) new_spf[j] = (uint32_t)i;
            }
        }
    }
    new_spf[0] = 0;
    new_spf[1] = 1;
    free(spf);
    spf = new_spf;
    spf_size = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* Ensure spf covers all numbers up to n */
    if (spf_size < n) build_spf(n);

    /* Factor n once */
    idx_t tmp = n;
    uint32_t primes_n[10];
    int exp_n[10];
    int cnt_n = 0;
    while (tmp > 1)
    {
        uint32_t p = spf[tmp];
        int e = 0;
        while (tmp % p == 0) { tmp /= p; ++e; }
        primes_n[cnt_n]   = p;
        exp_n[cnt_n++]    = e;
    }

    seq_t sum = 0;
    /* Enumerate k = 2 … n (k=1 never contributes) */
    for (idx_t k = 2; k <= n; ++k)
    {
        /* Factor k */
        idx_t ktmp = k;
        uint32_t primes_k[8];
        int exp_k[8];
        int cnt_k = 0;
        while (ktmp > 1 && cnt_k < 8)
        {
            uint32_t p = spf[ktmp];
            int a = 0;
            while (ktmp % p == 0) { ktmp /= p; ++a; }
            primes_k[cnt_k]   = p;
            exp_k[cnt_k++]    = a;
        }

        bool counted = false;
        if (cnt_k == 1)          /* pure prime power k = p^a */
        {
            uint32_t p = primes_k[0];
            int a      = exp_k[0];
            int en = 0;
            for (int i = 0; i < cnt_n; ++i)
                if (primes_n[i] == p) { en = exp_n[i]; break; }

            /* k counted iff there exists a proper divisor that fails:
               For a pure power, this happens when e_n(p) < a-1 */
            if (en < a - 1) counted = true;
        }
        else                      /* composite k with at least two distinct primes */
        {
            /* Count if any prime power p^a is missing in n (exponent in n < a) */
            for (int i = 0; i < cnt_k; ++i)
            {
                uint32_t p = primes_k[i];
                int a      = exp_k[i];
                int en = 0;
                for (int j = 0; j < cnt_n; ++j)
                    if (primes_n[j] == p) { en = exp_n[j]; break; }
                if (en < a) { counted = true; break; }
            }
        }

        if (counted)
            sum += (seq_t)k;
    }

    return sum;
}
