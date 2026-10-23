/*
 * A107362  Sequence A={a(n),n=0,1,2,3,...} such that the subsequence
 *          S1={a(n)|n mod 5=0,3} is identical to A and S2=S\S1 (the
 *          complement of S1 in A) is identical to A except with the first
 *          term omitted.
 * https://oeis.org/A107362
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  eigen, nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=2, a(2)=1.
   For n>2 let m = n/5. Then
       n%5 == 0 : a(n) = a(2*m)
       n%5 == 1 : a(n) = a(n-2*m)
       n%5 == 2 : a(n) = a(n-2*m)
       n%5 == 3 : a(n) = a(2*m+1)
       n%5 == 4 : a(n) = a(n-2*m-1) */

seq_t a(idx_t n)
{
    while (n > (idx_t)2) {
        idx_t m = n / 5;
        switch (n % 5) {
        case 0: n = 2 * m; break;
        case 1: n = n - 2 * m; break;
        case 2: n = n - 2 * m; break;
        case 3: n = 2 * m + 1; break;
        case 4: n = n - 2 * m - 1; break;
        }
    }
    return (n == (idx_t)1) ? (seq_t)2 : (seq_t)1;
}
