/*
 * A110653  n^2 followed by n^3 followed by n followed by n^4.
 * https://oeis.org/A110653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = { floor((n+3)/4)^2 if n≡1 (mod 4), floor((n+2)/4)^3 if
 *            n≡2 (mod 4), floor((n-3)/4)+1 if n≡3 (mod 4), (n/4)^4 if n≡0
 *            (mod 4) }
 * Range:     u8 n <= 15, u16 n <= 63, u32 n <= 1023, u64 n <= 4000,
 *            u128 n <= 4000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result;
    uint64_t r = n & 3;            /* n % 4 */

    if (r == 0) {                  /* n ≡ 0 (mod 4)  →  (n/4)^4 */
        seq_t k = (seq_t)(n >> 2);  /* k = n/4   */
        result = k * k * k * k;    /* k^4       */
    } else if (r == 1) {           /* n ≡ 1 (mod 4)  →  ((n+3)/4)^2 */
        seq_t k = (seq_t)((n + 3ULL) >> 2); /* k = (n+3)/4 */
        result = k * k;            /* k^2       */
    } else if (r == 2) {           /* n ≡ 2 (mod 4)  →  ((n+2)/4)^3 */
        seq_t k = (seq_t)((n + 2ULL) >> 2); /* k = (n+2)/4 */
        result = k * k * k;        /* k^3       */
    } else {                       /* r == 3      →  ((n-3)/4)+1 */
        seq_t k = (seq_t)((n - 3ULL) >> 2); /* k = (n-3)/4 */
        result = k + 1;            /* k + 1     */
    }
    return result;
}
