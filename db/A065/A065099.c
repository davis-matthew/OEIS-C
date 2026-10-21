/*
 * A065099  Weight 5 level 11 cusp form with complex multiplication by
 *          Q(sqrt(11)) and trivial character.
 * https://oeis.org/A065099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(1)=1; the function is multiplicative. a(11^e)=121^e. For a
 *            prime p≠11 let r = p mod 11. If r∈{2,6,7,8,10} then a(p^e) =
 *            (1+(-1)^e)/2 * p^(2e) (zero when e is odd, otherwise p^(2e)). If
 *            r∈{1,3,4,5,9} then find integers x,y with 4p=y^2+11x^2. Set ap =
 *            y^4 - 4p y^2 + 2 p^2. For e≥1: a(p^e) is defi...
 * Range:     i8 n <= 14, i16 n <= 124, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard against 0 */
    /* -------- factorisation of n ----------------------------------- */
    idx_t temp = n;
    /* small static buffer for prime factors (max 16 for n≤10¹⁴, enough here) */
    struct { idx_t p; idx_t e; } fac[16];
    int faccnt = 0;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t cnt = 0;
            while (temp % p == 0) {
                temp /= p;
                ++cnt;
            }
            fac[faccnt].p = p;
            fac[faccnt].e = cnt;
            ++faccnt;
        }
    }
    if (temp > 1) {
        fac[faccnt].p = temp;
        fac[faccnt].e = 1;
        ++faccnt;
    }
    /* -------- compute multiplicative values ------------------------ */
    seq_t result = 1;
    for (int i = 0; i < faccnt; ++i) {
        idx_t p = fac[i].p;
        idx_t e = fac[i].e;
        /* compute a(p^e) */
        seq_t apw = 1;
        if (p == 11) {
            /* a(11^e) = 121^e */
            for (idx_t j = 0; j < e; ++j) apw *= (seq_t)121;
        } else {
            idx_t r = p % 11;
            /* check residue class */
            if (r == 2 || r == 6 || r == 7 || r == 8 || r == 10) {
                if (e & 1U) return 0;  /* odd exponent gives zero */
                /* a(p^e) = p^(2e) */
                for (idx_t j = 0; j < e; ++j) {
                    apw *= (seq_t)p;
                    apw *= (seq_t)p;
                }
            } else { /* r ∈ {1,3,4,5,9} */
                /* find integers x,y with 4p = y^2 + 11x^2 */
                seq_t y = 0, y2 = 0;
                for (int x = 0; 11 * x * x <= 4 * p; ++x) {
                    seq_t cand = 4 * p - 11 * x * x;
                    /* check if cand is a perfect square */
                    seq_t t = 0;
                    for (seq_t tt = 0; tt * tt <= cand; ++tt) {
                        if (tt * tt == cand) { t = tt; break; }
                    }
                    if (t != 0) { y = t; y2 = y * y; break; }
                }
                /* a(p) = y^4 - 4p y^2 + 2p^2 */
                seq_t y4 = y2 * y2;
                seq_t p2 = (seq_t)p * (seq_t)p;
                seq_t term2 = 4 * (seq_t)p * y2;        /* 4p y^2 */
                seq_t term3 = 2 * p2;                   /* 2p^2 */
                apw = y4 - term2 + term3;              /* a(p) */
                /* recurrence for higher powers */
                if (e == 1) {
                    /* apw already a(p) */
                } else {
                    seq_t prev_prev = 1;    /* a(p^0) */
                    seq_t prev = apw;       /* a(p^1) */
                    seq_t curr;
                    for (idx_t k = 2; k <= e; ++k) {
                        curr = apw * prev - (seq_t)p * (seq_t)p * (seq_t)p * (seq_t)p * prev_prev;
                        prev_prev = prev;
                        prev = curr;
                    }
                    apw = prev;
                }
            }
        }
        result *= apw;
    }
    return result;
}
