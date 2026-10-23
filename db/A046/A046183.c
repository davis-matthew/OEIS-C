/*
 * A046183  Octagonal triangular numbers.
 * https://oeis.org/A046183
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(1)=1, a(2)=21, a(3)=11781, a(4)=203841;
 * For n>=5, a(n)=9602*a(n-2) - a(n-4) + 2200.
 * (This follows from the OEIS recurrence a(n)=a(n-1)+9602*a(n-2)-9602*a(n-3)-a(n-4)+a(n-5).)
 */
seq_t a(idx_t n)
{
    /* Handle the first four terms explicitly */
    if (n == 1) return 1;
    if (n == 2) return 21;
    if (n == 3) return 11781;
    if (n == 4) return 203841;

    /* Remaining terms are computed from the even/odd subsequences */
    /* Each subsequence satisfies the same second‑order linear recurrence */
    if (n & 1) {
        /* Odd indices: a(2k+1), k>=1 */
        const seq_t a1  = 1;
        const seq_t a3  = 11781;
        idx_t k_max = (n - 1) / 2;          /* desired k such that n = 2k+1 */
        seq_t prev4 = a1;                   /* a(2(k-1)+1)  */
        seq_t prev2 = a3;                   /* a(2k-1)      */
        for (idx_t k = 2; k <= k_max; ++k) { /* compute a(2k+1) iteratively */
            seq_t next = ((seq_t)9602) * prev2 - prev4 + ((seq_t)2200);
            prev4 = prev2;
            prev2 = next;
        }
        return prev2;                       /* a(n) */
    } else {
        /* Even indices: a(2k), k>=1 */
        const seq_t a2  = 21;
        const seq_t a4  = 203841;
        idx_t k_max = n / 2;                 /* desired k such that n = 2k */
        seq_t prev4 = a2;                   /* a(2(k-1))  */
        seq_t prev2 = a4;                   /* a(2k-2)    */
        for (idx_t k = 3; k <= k_max; ++k) { /* compute a(2k) iteratively */
            seq_t next = ((seq_t)9602) * prev2 - prev4 + ((seq_t)2200);
            prev4 = prev2;
            prev2 = next;
        }
        return prev2;                       /* a(n) */
    }
}
