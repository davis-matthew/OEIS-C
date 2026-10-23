/*
 * A014482  Inverse of 473rd cyclotomic polynomial.
 * https://oeis.org/A014482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = floor(n/473)+1 - floor((n-11)/473)-1 (if n>=11) -
 *            floor((n-43)/473)-1 (if n>=43) + floor((n-54)/473)-1 (if n>=54)
 *            This simplifies to a(n) = count(n) - count(n-11) - count(n-43) +
 *            count(n-54) where count(m) = ⌊m/473⌋+1 for m>=0 and 0 otherwise.
 *            The sequence is periodic with period 473
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count function: floor(m/473)+1 for m>=0.  Formula uses
     * negative indices as zero by guarding in the code. */
    long long cnt_n   = (long long)(n / 473ULL) + 1LL;
    long long cnt_n11 = 0;
    if (n >= 11ULL) {
        cnt_n11 = (long long)((n - 11ULL) / 473ULL) + 1LL;
    }
    long long cnt_n43 = 0;
    if (n >= 43ULL) {
        cnt_n43 = (long long)((n - 43ULL) / 473ULL) + 1LL;
    }
    long long cnt_n54 = 0;
    if (n >= 54ULL) {
        cnt_n54 = (long long)((n - 54ULL) / 473ULL) + 1LL;
    }

    long long res = cnt_n - cnt_n11 - cnt_n43 + cnt_n54;
    return (seq_t)res;
}
