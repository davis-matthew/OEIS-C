/*
 * A235566  Number of (n+1) X (1+1) 0..1 arrays with the difference of the
 *          upper and lower median value of each 2 X 2 subblock in
 *          lexicographically nondecreasing order rowwise and columnwise.
 * https://oeis.org/A235566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) - 10*a(n-3) + 8*a(n-4) Initial values:
 *            a(1) = 16, a(2) = 50, a(3) = 144, a(4) = 398
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 44, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* OEIS offset is 1.  Behavior for n < 1 is irrelevant. */
    if (n == 0) return 0;
    if (n == 1) return 16;
    if (n == 2) return 50;
    if (n == 3) return 144;
    if (n == 4) return 398;

    /* Store the last four values: a(n-1), a(n-2), a(n-3), a(n-4) */
    seq_t a_nm1 = 398; /* a(4) */
    seq_t a_nm2 = 144; /* a(3) */
    seq_t a_nm3 = 50;  /* a(2) */
    seq_t a_nm4 = 16;  /* a(1) */
    seq_t curr;

    /* Compute iteratively until we reach n */
    for (idx_t i = 5; i <= n; ++i) {
        curr = 4 * a_nm1;            /* 4*a(n-1) */
        curr += 8 * a_nm4;           /* + 8*a(n-4) */
        curr -= a_nm2;               /* - a(n-2) */
        curr -= 10 * a_nm3;          /* - 10*a(n-3) */

        /* Shift the window */

        a_nm4 = a_nm3;  /* a(n-3) becomes a(n-4) */
        a_nm3 = a_nm2;  /* a(n-2) becomes a(n-3) */
        a_nm2 = a_nm1;  /* a(n-1) becomes a(n-2) */
        a_nm1 = curr;   /* new value becomes a(n-1) */
    }
    return curr;
}
