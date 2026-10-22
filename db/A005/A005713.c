/*
 * A005713  Define strings S(0)=0, S(1)=11, S(n) = S(n-1)S(n-2); iterate.
 * https://oeis.org/A005713
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n-th digit of S(infinity): S(0)=0, S(1)=11, S(n)=S(n-1)S(n-2) */

seq_t a(idx_t n) {
    idx_t fib[100];
    fib[0] = 1;
    fib[1] = 2;
    for (int i = 2; i < 100; i++) {
        fib[i] = fib[i-1] + fib[i-2];
    }
    
    int k = 1;
    while (k < 99 && fib[k] <= n) k++;
    
    while (k > 1) {
        if (n < fib[k-1]) {
            k--;
        } else {
            n -= fib[k-1];
            k -= 2;
        }
    }
    
    return (k == 0) ? 0 : 1;
}
