/*
 * A079987  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=3, r=3, I={-1,0,2}.
 * https://oeis.org/A079987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A079987: Number of permutations satisfying -3 ≤ p(i)-i ≤ 3
   and p(i)-i ∉ { -1, 0, 2 }, for i = 1..n. 
   The sequence obeys the linear recurrence (order 18):

   a(n) =  2*a(n-1) - 2*a(n-2) + 3*a(n-3) - 2*a(n-4) + 4*a(n-5)
          - 1*a(n-6) - 2*a(n-7) + 4*a(n-8) - 5*a(n-9) + 4*a(n-10)
          - 6*a(n-11) + 3*a(n-12) - 2*a(n-14) + 1*a(n-15)
          - 2*a(n-16) + 2*a(n-17) - 1*a(n-18)

   Initial values (n = 0..17) are provided below. */
seq_t a(idx_t n)
{
    /* initial terms a(0)..a(17) */
    static const seq_t init[18] = {
        (seq_t)1,  (seq_t)0,  (seq_t)0,  (seq_t)1,
        (seq_t)2,  (seq_t)3,  (seq_t)5,  (seq_t)7,
        (seq_t)15, (seq_t)29, (seq_t)49, (seq_t)84,
        (seq_t)149,(seq_t)268,(seq_t)484,(seq_t)855,
        (seq_t)1508,(seq_t)2684
    };
    if (n < 18)
        return init[n];

    /* cyclic buffer of size 18 holding the last 18 values */
    seq_t buf[18];
    for (int i = 0; i < 18; ++i)
        buf[i] = init[i];

    /* compute successive terms up to n */
    for (idx_t i = 18; i <= n; ++i) {
        int i0 = (int)(i % 18);           /* index for a(i) */
        int i1 = (int)((i - 1) % 18);     /* a(i-1) */
        int i2 = (int)((i - 2) % 18);     /* a(i-2) */
        int i3 = (int)((i - 3) % 18);     /* a(i-3) */
        int i4 = (int)((i - 4) % 18);     /* a(i-4) */
        int i5 = (int)((i - 5) % 18);     /* a(i-5) */
        int i6 = (int)((i - 6) % 18);     /* a(i-6) */
        int i7 = (int)((i - 7) % 18);     /* a(i-7) */
        int i8 = (int)((i - 8) % 18);     /* a(i-8) */
        int i9 = (int)((i - 9) % 18);     /* a(i-9) */
        int i10 = (int)((i - 10) % 18);   /* a(i-10) */
        int i11 = (int)((i - 11) % 18);   /* a(i-11) */
        int i12 = (int)((i - 12) % 18);   /* a(i-12) */
        int i14 = (int)((i - 14) % 18);   /* a(i-14) */
        int i15 = (int)((i - 15) % 18);   /* a(i-15) */
        int i16 = (int)((i - 16) % 18);   /* a(i-16) */
        int i17 = (int)((i - 17) % 18);   /* a(i-17) */

        /* Preserve the old value of a(i-18) which is at buf[i0] */
        seq_t old_val = buf[i0];

        /* Compute a(i) using the recurrence, ordering additions first
           to avoid temporary underflow in unsigned arithmetic. */
        seq_t new_val = (seq_t)2 * buf[i1];          /* +2*a(i-1) */
        new_val -= (seq_t)2 * buf[i2];              /* -2*a(i-2) */
        new_val += (seq_t)3 * buf[i3];              /* +3*a(i-3) */
        new_val -= (seq_t)2 * buf[i4];              /* -2*a(i-4) */
        new_val += (seq_t)4 * buf[i5];              /* +4*a(i-5) */
        new_val -= buf[i6];                         /* -1*a(i-6) */
        new_val -= (seq_t)2 * buf[i7];              /* -2*a(i-7) */
        new_val += (seq_t)4 * buf[i8];              /* +4*a(i-8) */
        new_val -= (seq_t)5 * buf[i9];              /* -5*a(i-9) */
        new_val += (seq_t)4 * buf[i10];             /* +4*a(i-10) */
        new_val -= (seq_t)6 * buf[i11];             /* -6*a(i-11) */
        new_val += (seq_t)3 * buf[i12];             /* +3*a(i-12) */
        new_val -= (seq_t)2 * buf[i14];             /* -2*a(i-14) */
        new_val += buf[i15];                        /* +1*a(i-15) */
        new_val -= (seq_t)2 * buf[i16];             /* -2*a(i-16) */
        new_val += (seq_t)2 * buf[i17];             /* +2*a(i-17) */
        new_val -= old_val;                         /* -1*a(i-18) */

        buf[i0] = new_val;  /* store the computed value */
    }

    return buf[(int)(n % 18)];
}
