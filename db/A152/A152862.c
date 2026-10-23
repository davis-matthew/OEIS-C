/*
 * A152862  Triangle read by rows: vector recursion: s=5;
 *          v(n)={s^(n+1),s^(n+1)-Sum[s^i,{i,2,n}],s^n,...,-1}/s^2.
 * https://oeis.org/A152862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, sign
 * Formula:   A152862 is a triangle read by rows. For n>0 let r be such that
 *            r(r+1)/2 <= n < (r+1)(r+2)/2 (i.e. r is the row index). Let c =
 *            n - r(r+1)/2 (column, 0‑based). Then: r == 0 -> a(n) = 1 c == 0
 *            -> a(n) = 5^(r-1) c == 1 -> a(n) = -(3*5^(r+1)+25)/100 c >= 2 ->
 *            a(n) = -5^(r-c) This reproduces the OEIS...
 * Range:     i8 n <= 6, i16 n <= 21, i32 n <= 47, i64 n <= 47, i128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Computes the n‑th term of A152862 (zero‑based).  */
seq_t a(idx_t n)
{
    /* ---- Find row r and column c (0‑based) for index n ---- */
    idx_t lo = 0, hi = n;                 /* hi large enough; works even for n==0 */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        __int128 m = (__int128)(mid + 1) * (mid + 2) / 2;
        if (m > n) hi = mid;
        else       lo = mid + 1;
    }
    idx_t r = hi;                         /* row index */
    __int128 prev128 = (__int128)r * (r + 1) / 2;
    idx_t prev = (idx_t)prev128;          /* terms before row r */
    idx_t c = n - prev;                   /* column within the row */

    /* ---- Pre‑compute powers of 5 up to index r+1 ---- */
    static seq_t *pows = NULL;            /* powers of 5: pows[i] = 5^i */
    static idx_t pow_cap = 0;             /* allocated capacity for pows   */
    static idx_t pow_len = 0;             /* current length in use         */

    idx_t need = r + 1;                   /* need up to 5^(r+1) */
    if (!pows) {
        pow_cap = 1;
        pows = (seq_t *)malloc(sizeof(seq_t) * pow_cap);
        pows[0] = (seq_t)1;
        pow_len = 1;
    }
    while (pow_len <= need) {
        if (pow_len == pow_cap) {
            pow_cap *= 2;
            pows = (seq_t *)realloc(pows, sizeof(seq_t) * pow_cap);
        }
        pows[pow_len] = pows[pow_len - 1] * (seq_t)5;
        pow_len++;
    }

    /* ---- Compute requested term ---- */
    if (r == 0) {
        return (seq_t)1;
    }
    if (c == 0) {
        return pows[r - 1];
    }
    if (c == 1) {
        /* second term:  -(3*5^(r+1)+25)/100 */
        seq_t val = ((seq_t)3 * pows[r + 1] + (seq_t)25) / (seq_t)100;
        return -val;
    }
    /* remaining terms: -5^(r-c) */
    return -pows[r - c];
}
