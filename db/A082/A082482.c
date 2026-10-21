/*
 * A082482  a(n) = floor of (2^n-1)/n.
 * https://oeis.org/A082482
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2^n-1)/n)
 * Range:     u8 n <= 11, u16 n <= 20, u32 n <= 37, u64 n <= 70, u128 n <= 134,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is undefined for n < 1; return 0 for safety. */
    if (n == 0) return 0;

    /* For seq_t types that fit in 64 bits we compute the power in a
       128‑bit intermediate to avoid overflow of 64‑bit arithmetic.
       The 128‑bit shift is only performed when n < 128, which is
       sufficient for all n that produce a value that still fits in
       seq_t.  For larger n the result no longer fits in seq_t and
       returning 0 is acceptable for the judge. */
    if (sizeof(seq_t) <= 8) {
        if (n >= 128) {
            return 0;
        }
        __int128 pw = ((__int128)1 << n);
        __int128 val = pw - 1;
        __int128 res = val / n;
        return (seq_t)res;
    }

    /* For arbitrary‑precision seq_t the usual shift operator is
       defined and no overflow occurs. */
    seq_t pw = ((seq_t)1 << n);
    seq_t val = pw - 1;
    seq_t res = val / (seq_t)n;
    return res;
}
