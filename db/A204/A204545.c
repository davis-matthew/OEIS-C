/*
 * A204545  Symmetric matrix: f(i,j)=floor[(i+j+3)/4]-floor[(i+j+1)/4], by
 *          (constant) antidiagonals.
 * https://oeis.org/A204545
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if the least integer m with m(m-1)/2 ≥ n satisfies m ≡
 *            1 or 2 (mod 4); otherwise a(n) = 0
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the diagonal index m for a given position n in the
   matrix read by antidiagonals.  m is the smallest integer ≥ 2 with
   m(m-1)/2 ≥ n.  The function uses only integer arithmetic. */
static inline idx_t diagonal_of(idx_t n)
{
    idx_t low  = 2;
    idx_t high = 2;

    /* Increase upper bound until it covers position n. */
    while ((unsigned __int128)high * (high - 1) / 2 < n) {
        high <<= 1;
        if (high == 0) {          /* overflow guard: use maximum */
            high = UINT64_MAX;
            break;
        }
    }

    /* Binary search in [low, high]. */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        unsigned __int128 val = (unsigned __int128)mid * (mid - 1) / 2;
        if (val >= n)
            high = mid;
        else
            low  = mid + 1;
    }
    return low;                    /* m */
}

/* Sequence A204545: the entry at linear index n in the matrix. */
seq_t a(idx_t n)
{
    idx_t m    = diagonal_of(n);
    idx_t mod  = m & 3;   /* fast mod 4 */

    return (mod == 1 || mod == 2) ? (seq_t)1 : (seq_t)0;
}
