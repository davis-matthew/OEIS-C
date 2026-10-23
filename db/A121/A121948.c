/*
 * A121948  Floor of n-th 3-almost prime / n.
 * https://oeis.org/A121948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = floor(A014612(n)/n)
 * Range:     u8 n <= 60, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A014612(n) is the n-th 3‑almost prime (number with exactly three prime factors, counted with multiplicity)  */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* static state to cache the computed 3‑almost primes */
    static seq_t *three_almost = NULL;   /* array of discovered 3‑almost primes   */
    static size_t three_size = 0;        /* number of primes stored  */
    static size_t three_cap  = 0;        /* current capacity of the array       */
    static size_t sieve_limit = 0;       /* highest number that has been sieved  */

    /* Increase sieve size until we have at least n 3‑almost primes */
    while (three_size < (size_t)n)
    {
        /* pick new limit: start with 100 000, double each iteration */
        size_t new_limit = sieve_limit ? sieve_limit * 2 : 100000;

        /* --- Compute smallest prime factor (spf) for [2 .. new_limit] --- */
        uint32_t *spf = (uint32_t *)malloc((new_limit + 1) * sizeof(uint32_t));
        if (!spf) return 0;  /* out of memory */
        for (size_t i = 0; i <= new_limit; ++i) spf[i] = 0;

        for (size_t i = 2; i <= new_limit; ++i)
        {
            if (spf[i] == 0)       /* i is prime */
            {
                for (size_t j = i; j <= new_limit; j += i)
                    if (spf[j] == 0) spf[j] = (uint32_t)i;
            }
        }

        /* --- Count prime factors (Omega) and collect 3‑almost primes --- */
        uint8_t *omega = (uint8_t *)malloc((new_limit + 1) * sizeof(uint8_t));
        if (!omega) { free(spf); return 0; }
        omega[0] = omega[1] = 0;

        /* temporary list for new found primes */
        size_t new_cap  = 0;
        size_t new_size = 0;
        seq_t *new_list = NULL;

        for (size_t i = 2; i <= new_limit; ++i)
        {
            omega[i] = 1 + omega[i / spf[i]];
            if (omega[i] == 3)
            {
                if (new_size == new_cap)
                {
                    new_cap = new_cap ? new_cap * 2 : 4;
                    new_list = (seq_t *)realloc(new_list, new_cap * sizeof(seq_t));
                    if (!new_list) { free(spf); free(omega); return 0; }
                }
                new_list[new_size++] = (seq_t)i;
            }
        }

        free(spf);
        free(omega);

        /* Replace the old list with the new one */
        free(three_almost);
        three_almost = new_list;
        three_size   = new_size;
        three_cap    = new_cap;
        sieve_limit  = new_limit;
    }

    /* Now three_almost contains at least n elements */
    seq_t value = three_almost[n - 1];
    seq_t result = value / (seq_t)n;   /* integer division floors automatically */

    return result;
}
