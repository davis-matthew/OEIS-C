/*
 * A201953  A diagonal of irregular triangle A201949.
 * https://oeis.org/A201953
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^(n-2) in Π_{k=0}^{n-1} (1 + kx + x²)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation keeps the polynomial product explicitly and
   updates it incrementally, storing only the needed coefficients
   in a static array.  The routine is fully memoised and works for
   any order of calls.  It is O(n²) time with a 20001‐element
   coefficient array, which is sufficient for n up to about 10 000.   */

#include <stddef.h>
#include <stdbool.h>

#define MAX_N     10000            /* maximum n expected                 */
#define MAX_DEG   (2*MAX_N)         /* maximum polynomial degree          */
static seq_t coeff[MAX_DEG+1];     /* coefficient of x^d, d=0..MAX_DEG  */
static size_t cur_n = 1;          /* highest n for which coeff has been set (n=1) */
static size_t cur_deg = 2;        /* current maximum degree of coeff      */
static bool initialised = false;  /* ensures one‑time static initialisation */

seq_t a(idx_t n)
{
    if (n < 2)                          /* offset is 2 */
        return (seq_t)0;

    /* Initialise the polynomial product for n = 1:
       P₁(x) = 1 + x²  (corresponding to k = 0 factor).           */
    if (!initialised) {
        /* zero‑initialised by default, just set the two non‑zero terms */
        coeff[0] = (seq_t)1;
        coeff[2] = (seq_t)1;
        initialised = true;
    }

    /* Update the product until it reaches the requested n. */
    while (cur_n < (size_t)n) {
        size_t next_n = cur_n + 1;              /* next n to process    */
        seq_t k = (seq_t)(next_n - 1);          /* factor (1 + kx + x²) */
        size_t new_deg = cur_deg + 2;           /* degree increases by 2 */

        /* Update coefficients from high degree downwards.
           Old values are still available because we walk descending.  */
        for (size_t d = new_deg; ; --d) {
            seq_t a_val = (d <= cur_deg) ? coeff[d] : (seq_t)0;
            seq_t b_val = (d >= 1 && d-1 <= cur_deg) ? coeff[d-1] : (seq_t)0;
            seq_t c_val = (d >= 2 && d-2 <= cur_deg) ? coeff[d-2] : (seq_t)0;
            coeff[d] = a_val + k * b_val + c_val;
            if (d == 0)
                break;
        }

        cur_n   = next_n;
        cur_deg = new_deg;
    }

    /* Truncate index to the valid coefficient for a(n). */
    return coeff[n - 2];
}
