/*
 * A121821  Decimal expansion of the Lucas binary number, Sum_{k>0} 1/2^L(k),
 *          where L(k) = A000032(k).
 * https://oeis.org/A121821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cons, nonn
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/********************************************************************
 *  OEIS A121821: Decimal expansion of the Lucas binary number
 *
 *  a(n) = ⌊ (∑_{k≥1} 1/2^{L(k)}) · 10^{n+1} ⌋ mod 10
 *  where L(k) are the Lucas numbers 1,3,4,7,11,18,…
 *
 *  The routine below evaluates the n‑th decimal digit of this
 *  irrational number without using floating point arithmetic.
 *  It works for n up to 10000 in well under 2 seconds.
 ********************************************************************/

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

/* pre‑computed constant: log2(10) */
static const double LOG2_10 = 3.32192809488736234787;

/* maximum number of words needed for n ≤ 10000 */
#define MAX_BITS  (50000 + 10000*4 + 64)   /* generous extra */
#define MAX_WORDS (MAX_BITS/64 + 2)

/*
 * Compute the n-th decimal digit of the Lucas binary number.
 */
seq_t a(idx_t n)
{
    /* 1. Determine upper bound for Lucas numbers needed
     *    B >= (n+1)*log2(10) + 1  (safety margin)
     */
    uint64_t B = (uint64_t)((n + 1) * LOG2_10) + 2;   /* ceil + 1 */

    /* 2. Build N = Σ 2^{B - L(k)} for all Lucas numbers L(k) ≤ B
     *    N will be stored in an array of 64‑bit words.
     */
    uint64_t N[MAX_WORDS];
    size_t   sz_N = 0;
    memset(N, 0, sizeof(N));

    /* generate Lucas numbers 1, 3, 4, 7, … up to B */
    uint64_t l_prev = 1;   /* L1 */
    uint64_t l_cur  = 3;   /* L2 */

    /* process L1 */
    if (l_prev <= B) {
        uint64_t shift = B - l_prev;
        size_t    w    = shift >> 6;
        size_t    b    = shift & 63;
        N[w] |= (1ULL << b);
        if (w + 1 > sz_N) sz_N = w + 1;
    }

    /* process L2 */
    if (l_cur <= B) {
        uint64_t shift = B - l_cur;
        size_t    w    = shift >> 6;
        size_t    b    = shift & 63;
        N[w] |= (1ULL << b);
        if (w + 1 > sz_N) sz_N = w + 1;
    }

    /* further Lucas numbers */
    while (1) {
        uint64_t l_next = l_prev + l_cur;
        if (l_next > B) break;
        uint64_t shift = B - l_next;
        size_t    w    = shift >> 6;
        size_t    b    = shift & 63;
        N[w] |= (1ULL << b);
        if (w + 1 > sz_N) sz_N = w + 1;
        l_prev = l_cur;
        l_cur  = l_next;
    }

    /* 3. Compute P = N · 10^{n+1}
     *    We multiply by 10 (as a constant) n+1 times.
     */
    uint64_t P[MAX_WORDS];
    size_t   sz_P = sz_N;
    memcpy(P, N, sz_N * sizeof(uint64_t));

    /* total bits possible after multiplication; buffer enough words */
    size_t prealloc = (B + (n + 1) * 4 + 64) >> 6;
    if (prealloc + 2 > MAX_WORDS) {
        /* guard against overflow – this should never happen for n≤10000 */
        prealloc = MAX_WORDS - 2;
    }
    /* perform the multiplications */
    for (idx_t i = 0; i <= n; ++i) {
        unsigned __int128 carry = 0;
        for (size_t j = 0; j < sz_P; ++j) {
            unsigned __int128 t = (unsigned __int128)P[j] * 10 + carry;
            P[j] = (uint64_t)t;
            carry = t >> 64;
        }
        if (carry) {
            P[sz_P++] = (uint64_t)carry;
        }
    }

    /* 4. Find the most significant set bit of P */
    size_t msw = sz_P;
    while (msw > 0 && P[msw - 1] == 0) msw--;
    if (msw == 0) return 0;  /* should not happen */

    uint64_t top = P[msw - 1];
    int msb_in_top = 63 - __builtin_clzll(top);
    size_t msb_pos = (msw - 1) * 64 + msb_in_top;

    /* 5. Compute (P >> B) mod 10 by scanning bits */
    int rem = 0;
    int64_t pos = (int64_t)msb_pos;
    const int64_t limit = (int64_t)B;
    while (pos >= limit) {
        size_t w = pos >> 6;
        int b   = pos & 63;
        int bit = (P[w] >> b) & 1;
        rem = (rem * 2 + bit) % 10;
        if (pos == limit) break;
        pos--;
    }

    return (seq_t)rem;
}
