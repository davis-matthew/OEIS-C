/*
 * A083678  Numbers m = d_1 d_2 ... d_k (in base 10) with properties that k is
 *          even and d_i + d_{k+1-i} = 10 for all i.
 * https://oeis.org/A083678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) is the decimal integer whose left half is the
 *            (n‑Σ_{i<k}9^i)-th base‑9 number (digits 1–9) and whose right
 *            half is the reverse complement to 10 of that left half
 * Range:     u8 n <= 9, u16 n <= 90, u32 n <= 819, u64 n <= 819,
 *            u128 n <= 819, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find the length 2m of the n-th term */
    unsigned __int128 sum = 0;      /* Σ_{i=1}^{m-1}9^i           */
    unsigned __int128 pow = 9;      /* 9^m                         */
    int m = 1;                      /* current pair count          */
    while (1) {
        if (sum + pow >= n) break;  /* enough terms found          */
        sum += pow;                 /* add 9^m to cumulative sum  */
        pow *= 9;                   /* next power of 9            */
        ++m;
    }

    /* offset of n inside the block of length 2m (0‑based) */
    unsigned __int128 offset = (unsigned __int128)n - sum - 1;

    /* decode offset into base‑9 digits (0‑8) for the left half */
    int digits[64]; /* m ≤ 20 for n ≤ 2^64‑1, so 64 is safe */
    for (int i = m - 1; i >= 0; --i) {
        digits[i] = (int)(offset % 9);
        offset /= 9;
    }

    /* build the result */
    seq_t result = 0;
    for (int i = 0; i < m; ++i) {
        result = result * 10 + (digits[i] + 1); /* left half, digits 1–9 */
    }
    for (int i = m - 1; i >= 0; --i) {
        result = result * 10 + (10 - (digits[i] + 1)); /* right half */
    }
    return result;
}
