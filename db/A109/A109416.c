/*
 * A109416  Triangle read by row where n-th row contains the least n primes of
 *          form 4k+1 if n is odd and of form 4k+3 if n is even such that
 *          sequence is always increasing.
 * https://oeis.org/A109416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the n‑th element of the triangle where each odd * row
 *            contains the least unused primes ≡1 (mod 4) and each even * row
 *            contains the least unused primes ≡3 (mod 4), preserving a *
 *            strictly increasing sequence
 * Range:     u8 n <= 30, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   is_prime, push_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/push_seq.h"

/* Return the n-th term of OEIS sequence A109416 */
seq_t a(idx_t n)
{
    /* Static memoized state */
    static seq_t      *seq       = NULL;   /* array of computed terms          */
    static idx_t      seq_len   = 0;       /* number of terms stored           */
    static idx_t      seq_cap   = 0;       /* current allocated capacity       */
    static uint64_t   last_prime = 1;      /* last prime appended (not in seq) */
    static idx_t      next_row  = 1;       /* next row (1‑based) to generate   */

    /* Generate terms until we have at least n of them */
    while (seq_len < (idx_t)n)
    {
        /* Desired residue for this row: 1 for odd rows, 3 for even rows */
        unsigned int residue = ((next_row & 1U) == 1U) ? 1U : 3U;

        /* Number of primes to collect for this row */
        idx_t needed = next_row;

        /* Scan integers starting after the last prime already used */
        uint64_t cand = last_prime;

        while (needed > 0)
        {
            ++cand;  /* move to next integer */

            if ((cand & 3U) == residue && is_prime(cand))
            {
                /* Append prime to the array */
                push_seq(&seq, &seq_cap, &seq_len, (seq_t)cand);

                /* Record this prime for subsequent searches */
                last_prime = cand;

                /* One slot of the current row satisfied */
                --needed;
            }
        }

        /* Advance to the next row for future generations */
        ++next_row;
    }

    /* Sequence array is zero‑indexed; n is 1‑based */
    return seq[(size_t)(n - 1)];
}
