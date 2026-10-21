/*
 * A059036  In a triangle of numbers (such as that in A059032, A059033,
 *          A059034) how many entries lie above position (n,k)? Answer: T(n,k)
 *          = (n+1)*(k+1)-1 (n >= 0, k >= 0).
 * https://oeis.org/A059036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) lists T(n,k) = (n+1)*(k+1)-1 along antidiagonals of the
 *            infinite triangle. The index n is 0‑based. The function
 *            determines the diagonal s (where s = n+k) containing the entry
 *            and the offset j within that diagonal (j = i for the pair
 *            (i,s-i)). Then a(n) = (j+1)*(s-j+1)-1. All arithmetic that...
 * Range:     u8 n <= 507, u16 n <= 5150, u32 n <= 5150, u64 n <= 5150,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the diagonal s such that the total number of elements
       up to diagonal s-1 ≤ n < total up to s.  The total up to
       diagonal s is (s+1)*(s+2)/2. */
    idx_t lo = 0, hi = 1;
    /* Increase hi until the total up to diagonal hi exceeds n. */
    while (((__int128)(hi + 1) * (hi + 2) / 2) <= n) {
        if (hi > (UINT64_MAX >> 1)) {
            hi = UINT64_MAX;
            break;
        }
        hi <<= 1;
    }
    /* Binary search for the minimal s with total > n. */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        __int128 total = (__int128)(mid + 1) * (mid + 2) / 2;
        if (total <= n)
            lo = mid + 1;
        else
            hi = mid;
    }
    idx_t s = lo;                         /* diagonal index */
    __int128 prev128 = (__int128)s * (s + 1) / 2; /* elements before diagonal s */
    idx_t prev = (idx_t)prev128;
    idx_t j = n - prev;                   /* position within diagonal (0 ≤ j ≤ s) */

    /* Compute the value T(j, s-j).  Use 128‑bit intermediate to avoid
       overflow in the 64‑bit build; cast back at the end. */
    __int128 val128 = (__int128)(j + 1) * (s - j + 1) - 1;
    return (seq_t)val128;
}
