/*
 * A164938  a(n) = (n^5 - n)/10, which is always an integer.
 * https://oeis.org/A164938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (n^5 - n) / 10
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 133, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;            /* undefined for n<1, return 0 */
    seq_t sn = (seq_t)n;

#if defined(__SIZEOF_INT128__)   /* use 128‑bit intermediate if available */
    __uint128_t prod = 1;
    prod *= (unsigned __int128)(sn - 1);
    prod *= (unsigned __int128)sn;
    prod *= (unsigned __int128)(sn + 1);
    /* n*n fits in 64 bits for all required n (≤ 10⁴), so safe to cast */
    prod *= (unsigned __int128)(sn * sn + 1);
    prod /= 10;
    return (seq_t)prod;
#else
    /* fallback for environments without int128: compute with
       early division to avoid overflow. */
    /* product = (sn-1)*sn*(sn+1)*(sn*sn+1) / 10 */
    /* We perform the division by 2 and 5 as early as possible. */
    unsigned long long a = sn - 1;
    unsigned long long b = sn;
    unsigned long long c = sn + 1;
    unsigned long long d = sn * sn + 1;   /* fits in unsigned long long */

    /* divide by 2 if possible */
    if ((a & 1ULL) == 0) { a >>= 1; }
    else if ((b & 1ULL) == 0) { b >>= 1; }
    else { c >>= 1; }

    /* divide by 5 if possible */
    if (a % 5ULL == 0) { a /= 5ULL; }
    else if (b % 5ULL == 0) { b /= 5ULL; }
    else if (c % 5ULL == 0) { c /= 5ULL; }
    else { d /= 5ULL; }

    unsigned long long prod = a;
    prod *= b;
    prod *= c;
    prod *= d;
    return (seq_t)prod;
#endif
}
