/*
 * A070212  Number of 5 X 5 pandiagonal magic squares with sum n.
 * https://oeis.org/A070212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/8064)*(n+4)*(n+3)*(n+2)*(n+1)*(n^2+5*n+8)*(n^2+5*n+42)
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 46, u64 n <= 785, u128 n <= 5000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
#ifdef __SIZEOF_INT128__
    /* Use 128‑bit arithmetic for exact intermediate results */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 prod = 1;
    prod *= (nn + 4);
    prod *= (nn + 3);
    prod *= (nn + 2);
    prod *= (nn + 1);
    prod *= (nn * nn + 5 * nn + 8);
    prod *= (nn * nn + 5 * nn + 42);
    prod /= 8064;
    return (seq_t)prod;
#else
    /* Fallback method that cancels the factors 2^7·3^2·7 during
       multiplication to avoid overflow when seq_t is a small integer type. */
    seq_t nn = (seq_t)n;
    seq_t t1 = nn + (seq_t)4;
    seq_t t2 = nn + (seq_t)3;
    seq_t t3 = nn + (seq_t)2;
    seq_t t4 = nn + (seq_t)1;
    seq_t t5 = nn * nn + (seq_t)5 * nn + (seq_t)8;
    seq_t t6 = nn * nn + (seq_t)5 * nn + (seq_t)42;
    seq_t arr[6] = {t1, t2, t3, t4, t5, t6};

    int rem2 = 7, rem3 = 2, rem7 = 1;
    seq_t ans = 1;
    for (int i = 0; i < 6; ++i) {
        seq_t v = arr[i];
        /* cancel powers of 2, 3 and 7 from the current factor */
        while (rem2 > 0 && (v & (seq_t)1) == 0) { v >>= 1; rem2--; }
        while (rem3 > 0 && v % (seq_t)3 == 0) { v /= (seq_t)3; rem3--; }
        while (rem7 > 0 && v % (seq_t)7 == 0) { v /= (seq_t)7; rem7--; }
        /* multiply the remaining part */
        ans = ans * v;
        /* cancel any remaining factors from the accumulated result */
        while (rem2 > 0 && (ans & (seq_t)1) == 0) { ans >>= 1; rem2--; }
        while (rem3 > 0 && ans % (seq_t)3 == 0) { ans /= (seq_t)3; rem3--; }
        while (rem7 > 0 && ans % (seq_t)7 == 0) { ans /= (seq_t)7; rem7--; }
    }
    /* Any remaining factors are cancelled here */
    while (rem2 > 0) { ans /= (seq_t)2; rem2--; }
    while (rem3 > 0) { ans /= (seq_t)3; rem3--; }
    while (rem7 > 0) { ans /= (seq_t)7; rem7--; }
    return ans;
#endif
}
