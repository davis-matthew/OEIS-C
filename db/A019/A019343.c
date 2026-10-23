/*
 * A019343  Primes with primitive root 15.
 * https://oeis.org/A019343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factorize, is_prime_big, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A019343 Primes for which 15 is a primitive root.
 *  a(n) = the n-th prime p (n≥1) with ord_p(15)=p-1.
 *  The function builds a memoised list of such primes so that
 *  arbitrary call order is supported. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"
#include "../utils/factorize.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    if (n == 0)        /* outside the offset – behaviour unspecified */
        return 0;

    /* Memoisation of the sequence */
    static seq_t *table  = NULL;   /* array of found primes               */
    static size_t  cap    = 0;     /* current allocation capacity (in elems) */
    static size_t  count  = 0;     /* number of primes stored             */
    static uint64_t next  = 2;     /* next integer to test                */

    /* Ensure the table can hold at least n primes */
    if (count < n) {
        if (cap < n) {
            size_t new_cap = (cap == 0) ? (n * 2) : (cap * 2);
            seq_t *tmp = (seq_t *)realloc(table, new_cap * sizeof(seq_t));
            if (!tmp) return 0;        /* allocation failure – give up */
            table = tmp;
            cap   = new_cap;
        }

        /* Generate primes until we have n of them */
        while (count < n) {
            /* Skip non‑primes */
            if (!is_prime_big((seq_t)next)) {
                ++next;
                continue;
            }

            /* Skip primes dividing 15 (p=3 or 5) */
            if (next == 3 || next == 5) {
                ++next;
                continue;
            }

            /* Determine whether 15 is a primitive root modulo next.
             *  This is true iff for every prime factor q of (next-1),
             *  15^((next-1)/q) mod next ≠ 1.                */
            uint64_t prime_factors[64];
            int     powers[64];
            size_t  pf_cnt = factorize(next - 1, prime_factors, powers);

            bool primitive = true;
            for (size_t i = 0; i < pf_cnt && primitive; ++i) {
                uint64_t q   = prime_factors[i];
                uint64_t exp = (next - 1) / q;
                if (mod_pow((seq_t)15, exp, (seq_t)next) == (seq_t)1)
                    primitive = false;
            }

            if (primitive)
                table[count++] = (seq_t)next;

            ++next;
        }
    }

    return table[n - 1];
}
