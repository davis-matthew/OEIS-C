/*
 * A121578  Values m of number pairs (m,n) which yield associated matching
 *          times on the clock with interchanged hour and minute hands for
 *          corresponding n in A121577.
 * https://oeis.org/A121578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, nonn
 * Formula:   a(n) = ((n-1) / 12) + 12 * ((n-1) % 12)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Derived from: for n = 12*i + j (0 ≤ i < 12, 0 ≤ j < 12)   */
/*   then m = i + 12*j, and n is indexed from 1 so we use n-1. */

seq_t a(idx_t n)
{
    /* Handle the first element explicitly (offset = 1).  */
    if (n == 0) {
        return 0;
    }

    /* Convert the one‑based index to zero‑based.  */
    idx_t n0 = n - 1;

    /* Split the zero‑based index into i and j as described.  */
    idx_t i = n0 / 12;          /* i ∈ [0, ∞)  */
    idx_t j = n0 % 12;          /* j ∈ [0, 11]  */

    /* Compute m = i + 12*j in seq_t, avoiding any overflow
       beyond what seq_t can hold for the 64‑bit build.  */
    return (seq_t)i + (seq_t)12 * (seq_t)j;
}
