/*
 * A165131  Number of reduced words of length n in Coxeter group on 32
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^8 = I.
 * https://oeis.org/A165131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Assignment: Number of reduced words in the Coxeter group
   on 32 generators with relations S_i^2 = (S_i S_j)^8 = I.
   The generating function is
      (t^8 + 2t^7 + 2t^6 + 2t^5 + 2t^4 + 2t^3 + 2t^2 + 2t + 1)
      -------------------------------------------------------------
      (465t^8 - 30t^7 - 30t^6 - 30t^5 - 30t^4 - 30t^3 - 30t^2 - 30t + 1)

   From this we obtain the linear recurrence
      a(n) = 30*(a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7))
             - 465*a(n-8)          for n ≥ 9,
      a(8) = 30*(a7+…+a1) - 465*a0 + 1
   with the first 8 terms as given below.
   Exact arithmetic is performed in seq_t; unsigned arithmetic is safe
   because the subtrahend 465*a(n-8) is always ≤ the sum of the seven
   preceding terms for n ≥ 8.  */

seq_t a(idx_t n)
{
    /* Initial values a(0) … a(7)  */
    static const seq_t init[8] = {
        1ULL,          /* a(0) */
        32ULL,         /* a(1) */
        992ULL,        /* a(2) */
        30752ULL,      /* a(3) */
        953312ULL,     /* a(4) */
        29552672ULL,   /* a(5) */
        916132832ULL,  /* a(6) */
        28400117792ULL /* a(7) */
    };

    if (n < 8)
        return init[n];

    /*  s[0..6] hold a_{n-7} … a_{n-1} ,
        s[7]    holds a_{n-8} .   */
    seq_t s[8];
    s[0] = init[1]; /* a1  */
    s[1] = init[2]; /* a2  */
    s[2] = init[3]; /* a3  */
    s[3] = init[4]; /* a4  */
    s[4] = init[5]; /* a5  */
    s[5] = init[6]; /* a6  */
    s[6] = init[7]; /* a7  */
    s[7] = init[0]; /* a0  */

    for (idx_t i = 8; i <= n; ++i) {
        seq_t sum = 0;
        for (int k = 0; k < 7; ++k)
            sum += (seq_t)30 * s[k];

        seq_t next = sum - (seq_t)465 * s[7];

        if (i == 8)           /* add the constant term of the recurrence */
            next += 1;

        if (i == n)           /* desired term reached */
            return next;

        /* shift the window for the next iteration:
           a_{n-7} becomes a_{(n+1)-7} = a_{n-6}  … etc.
           a_{n-8} becomes the new a_{(n+1)-8}.      */
        seq_t tmp = s[0];
        for (int k = 0; k < 6; ++k)
            s[k] = s[k + 1];
        s[6] = next;   /* a_{n} becomes a_{(n+1)-1} */
        s[7] = tmp;    /* old a_{n-7} becomes a_{(n+1)-8} */
    }

    /* The loop always returns before reaching this point.  */
    return 0;
}
