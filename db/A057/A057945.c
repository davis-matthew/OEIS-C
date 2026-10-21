/*
 * A057945  Number of triangular numbers needed to represent n with greedy
 *          algorithm.
 * https://oeis.org/A057945
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 8742, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = number of triangular numbers needed to represent n
   with the greedy algorithm.  The algorithm repeatedly subtracts the
   largest triangular number not exceeding the current remainder. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    seq_t count = 0;
    while (n != 0) {
        /* binary search for the largest k with k*(k+1)/2 <= n */
        idx_t low = 0;
        idx_t high = n;                  /* k cannot exceed n */
        while (low < high) {
            idx_t mid = (low + high + 1) / 2;
            seq_t tri = (seq_t)mid * ((seq_t)mid + 1) / 2;
            if (tri <= n)
                low = mid;
            else
                high = mid - 1;
        }
        /* subtract the found triangular number and increment count */
        seq_t tri = (seq_t)low * ((seq_t)low + 1) / 2;
        n -= (idx_t)tri;
        ++count;
    }
    return count;
}
