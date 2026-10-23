/*
 * A186077  Index of Fibonacci and tribonacci numbers having the same last
 *          digit.
 * https://oeis.org/A186077
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th index i (i ≥ 1) where Fib[i] mod 10 == Trib[i]
 *            mod 10 * and that common last digit is odd. The Fibonacci
 *            numbers use F₁ = 1, F₂ = 1. * The Tribonacci numbers use Trib₁ =
 *            1, Trib₂ = 1, Trib₃ = 1 and Tribᵢ = Tribᵢ₋₁ + * Tribᵢ₋₂ +
 *            Tribᵢ₋₃ (mod 10). The function generates values lazil...
 * Range:     u8 n <= 33, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static state to memoise results across calls */
    static seq_t *seq   = NULL;
    static size_t seq_len = 0;
    static size_t seq_cap = 0;
    static idx_t  last_n_global = 0;
    static uint8_t trib_prev[3] = {1, 1, 1};      /* Trib[i-3], Trib[i-2], Trib[i-1] for next step */
    static uint8_t fib_cycle[60];
    static bool    initialized = false;

    if (n < 1) { return 0; }   /* undefined behaviour for n < 1 */

    /* Initialise Fibonacci period and sequence storage on first call */
    if (!initialized)
    {
        fib_cycle[0] = 1;   /* F₁ mod 10 */
        fib_cycle[1] = 1;   /* F₂ mod 10 */
        for (int i = 2; i < 60; ++i)
            fib_cycle[i] = (fib_cycle[i - 1] + fib_cycle[i - 2]) % 10;

        seq_cap = 256;
        seq     = (seq_t*)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
        initialized = true;
    }

    /* Extend the sequence until we have at least n terms */
    while (seq_len < (size_t)n)
    {
        ++last_n_global;

        /* Fibonacci last digit (using 1-based indexing) */
        uint8_t fib_mod = fib_cycle[(last_n_global - 1) % 60];

        /* Tribonacci last digit (mod 10) */
        uint8_t trib_mod;
        if (last_n_global <= 3)
        {
            trib_mod = 1;   /* Trib₁ = Trib₂ = Trib₃ = 1 */
        }
        else
        {
            trib_mod = (trib_prev[0] + trib_prev[1] + trib_prev[2]) % 10;
            /* shift the state for the next iteration */
            trib_prev[0] = trib_prev[1];
            trib_prev[1] = trib_prev[2];
            trib_prev[2] = trib_mod;
        }

        /* If the last digit is odd and matches, record the index */
        if ((fib_mod & 1) && fib_mod == trib_mod)
        {
            if (seq_len == seq_cap)
            {
                seq_cap = seq_cap ? seq_cap * 2 : 256;
                seq     = (seq_t*)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = (seq_t)last_n_global;
        }
    }

    return seq[n - 1];
}
