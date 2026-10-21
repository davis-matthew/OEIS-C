/*
 * A080458  a(1)=4; for n>1, a(n)=a(n-1) if n is already in the sequence,
 *          a(n)=a(n-1)+4 otherwise.
 * https://oeis.org/A080458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 + 4*(n-2 - floor((n-4)/4))
 * Range:     u8 n <= 84, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence can be described as: each positive integer t
   contributes one term 4*t, and if t is a multiple of 3 it contributes
   a second term 4*t.  Thus the number of terms up to value 4*t is
   count(t) = t + floor(t/3).  The nth term equals 4*t where t is the
   smallest integer with count(t) ≥ n.  This is found by binary search. */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset is 1, but guard just in case */

    idx_t lo = 1, hi = n;                 /* count(hi) >= n because hi + hi/3 >= hi */

    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        unsigned __int128 cnt = (unsigned __int128)mid + mid / 3;
        if (cnt >= n)
            hi = mid;
        else
            lo = mid + 1;
    }

    return (seq_t)4 * lo;
}
