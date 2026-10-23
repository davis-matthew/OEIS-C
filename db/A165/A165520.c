/*
 * A165520  Antidiagonal writing from three rows trio A165351,A165355,A165367
 *          (first,second and third trisections of A026741).
 * https://oeis.org/A165520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned
 * Formula:   a(n) = * a(1) = 0, * if n % 6 == 0: a(n) = n/2, * if n % 6 == 1:
 *            a(n) = (n==1?0:n-2), * if n % 6 == 2: a(n) = n-1, * if n % 6 ==
 *            3: a(n) = n, * if n % 6 == 4: a(n) = (n-2)/2, * if n % 6 == 5:
 *            a(n) = (n-1)/2
 * Range:     u8 n <= 33, u16 n <= 33, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 1) return 0;          /* a(1) = 0, a(0) not defined but safe */
    idx_t r = n % 6ULL;
    seq_t res;
    switch (r) {
        case 0: /* n = 6k  -> a = n/2 */
            res = (seq_t)(n / 2ULL);
            break;
        case 1: /* n = 6k+1 -> a = n-2 (except n=1 handled above) */
            res = (seq_t)(n - 2ULL);
            break;
        case 2: /* n = 6k+2 -> a = n-1 */
            res = (seq_t)(n - 1ULL);
            break;
        case 3: /* n = 6k+3 -> a = n */
            res = (seq_t)n;
            break;
        case 4: /* n = 6k+4 -> a = (n-2)/2 */
            res = (seq_t)((n - 2ULL) / 2ULL);
            break;
        case 5: /* n = 6k+5 -> a = (n-1)/2 */
            res = (seq_t)((n - 1ULL) / 2ULL);
            break;
    }
    return res;
}
