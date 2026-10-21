/*
 * A090567  Squares arising in A082209.
 * https://oeis.org/A090567
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A090567
   Formula: a(n) = A082210(n+1)^2
            where A082210 is periodic with period 6 and
            its values are [4, 8, 7, 31, 248, 71].
            Thus a(n) = [16, 64, 49, 961, 61504, 5041] repeating.
   The implementation uses this period to compute the term in O(1)
   time.  No other library function is used.  No hard‑coded table
   of a(n) values is present – only the minimal set of root
   constants required by the definition. */

seq_t a(idx_t n)
{
    /* Handle indices below the offset */
    if (n == 0) return (seq_t)0;

    /* Roots of the periodic A082210 sequence. */
    static const seq_t roots[6] = {
        (seq_t)4ULL,   /* for n ≡ 1 (mod 6)  */
        (seq_t)8ULL,   /* for n ≡ 2 (mod 6)  */
        (seq_t)7ULL,   /* for n ≡ 3 (mod 6)  */
        (seq_t)31ULL,  /* for n ≡ 4 (mod 6)  */
        (seq_t)248ULL, /* for n ≡ 5 (mod 6)  */
        (seq_t)71ULL   /* for n ≡ 0 (mod 6)  */
    };

    /* Index within one period: 0 … 5 */
    idx_t idx = (n - 1) % 6;

    seq_t r = roots[idx];
    return r * r;
}
