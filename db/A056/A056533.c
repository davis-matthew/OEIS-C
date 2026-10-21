/*
 * A056533  Even sieve: start with natural numbers, remove every 2nd term,
 *          remove every 4th term from what remains, remove every 6th term
 *          from what remains, etc.
 * https://oeis.org/A056533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A056533: Even sieve – remove every 2k-th term iteratively.
   The function builds the list of natural numbers and applies the sieve
   until no further removals are possible, then returns the n-th survivor. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* Safety for non‑positive indices */

    /* Estimate initial upper bound for the list.
       L ≈ n * (⌊log2 n⌋ + 2) * 2   (guarantees we get enough survivors)   */
    size_t bits = 64 - __builtin_clzll((uint64_t)n);   /* floor(log2 n) + 1 */
    size_t L = (size_t)n * (bits + 2) * 2;
    if (L < 10) L = 10;

    while (1) {
        /* Allocate an array of seq_t values from 1..L.  Use malloc directly
           to avoid any helper; seq_t is a plain integer type (or an
           arbitrary‑precision class). */ 
        seq_t *arr = (seq_t *)malloc(sizeof(seq_t) * L);
        if (!arr) return 0;   /* Allocation failure */

        for (size_t i = 0; i < L; ++i)
            arr[i] = (seq_t)(i + 1);

        size_t len = L;

        /* Apply the sieve: remove every 2k-th element for k = 1,2,... */
        for (size_t k = 1; 2 * k <= len; ++k) {
            size_t step = 2 * k;          /* 1‑based step size */
            size_t write = 0;            /* index for next kept element */

            for (size_t i = 0; i < len; ++i) {
                if (((i + 1) % step) != 0)   /* keep if not a multiple of step */
                    arr[write++] = arr[i];
            }
            len = write;                 /* new length after removal */
        }

        if (len >= (size_t)n) {        /* enough survivors found */
            seq_t res = arr[n - 1];    /* n‑th survivor (1‑based) */
            free(arr);
            return res;
        }

        /* Not enough survivors – increase the list size and retry. */
        free(arr);
        L = L * 2 + 10;
    }
}
