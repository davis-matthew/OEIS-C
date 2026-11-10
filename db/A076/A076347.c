/*
 * A076347  A076340(A005117(n)), real part of squarefree numbers mapped as
 *          defined in A076340, A076341.
 * https://oeis.org/A076347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = real part of f(s), s = A005117(n) (n-th squarefree
 *            number), f multiplicative with f(2)=(2,0), f(p)=(p-1,1) if p==1
 *            (mod 4), f(p)=(p+1,-1) if p==3 (mod 4); complex product
 *            (a,b)*(c,d)=(ac-bd, ad+bc)
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

static bool A076347_sqfree(uint64_t k) {
    if (k % 4 == 0) return false;
    for (uint64_t i = 3; i * i <= k; i += 2)
        if (k % (i * i) == 0) return false;
    return true;
}

static uint64_t A076347_nth_sqfree(idx_t n) {
    static uint64_t *cache = 0;
    static idx_t size = 0, cap = 0;
    if (n > size) {
        if (n > cap) {
            idx_t nc = cap ? cap : 64;
            while (nc < n) nc *= 2;
            cache = (uint64_t *)realloc(cache, (size_t)nc * sizeof(uint64_t));
            cap = nc;
        }
        uint64_t k = size ? cache[size - 1] : 0;
        for (idx_t i = size; i < n; i++) {
            do { k++; } while (!A076347_sqfree(k));
            cache[i] = k;
        }
        size = n;
    }
    return cache[n - 1];
}

/* signed add of sign-magnitude values */
static void A076347_sadd(seq_t am, bool an, seq_t bm, bool bn, seq_t *rm, bool *rn) {
    if (an == bn) { *rm = am + bm; *rn = an; }
    else if (am >= bm) { *rm = am - bm; *rn = an; }
    else { *rm = bm - am; *rn = bn; }
}

/* multiply (R,I) by (x, +-1); yneg = sign of imaginary part; x==0 flag handled separately */
static void A076347_mulp(seq_t *rm, bool *rn, seq_t *im, bool *in_, seq_t x, bool yneg) {
    seq_t t1m = *rm * x; bool t1n = *rn;
    seq_t t2m = *im;     bool t2n = !(*in_ != yneg);
    seq_t t3m = *rm;     bool t3n = (*rn != yneg);
    seq_t t4m = *im * x; bool t4n = *in_;
    seq_t nrm, nim; bool nrn, nin;
    A076347_sadd(t1m, t1n, t2m, t2n, &nrm, &nrn);
    A076347_sadd(t3m, t3n, t4m, t4n, &nim, &nin);
    *rm = nrm; *rn = nrn; *im = nim; *in_ = nin;
}

seq_t a(idx_t n) {
    uint64_t s = A076347_nth_sqfree(n);
    seq_t rm = 1, im = 0;
    bool rn = false, in_ = false;
    for (uint64_t p = 2; p * p <= s; p++) {
        if (s % p != 0) continue;
        s /= p;
        if (p == 2) { rm *= 2; im *= 2; }
        else if (p % 4 == 1) A076347_mulp(&rm, &rn, &im, &in_, (seq_t)(p - 1), false);
        else A076347_mulp(&rm, &rn, &im, &in_, (seq_t)(p + 1), true);
    }
    if (s > 1) {
        uint64_t p = s;
        if (p == 2) { rm *= 2; im *= 2; }
        else if (p % 4 == 1) A076347_mulp(&rm, &rn, &im, &in_, (seq_t)(p - 1), false);
        else A076347_mulp(&rm, &rn, &im, &in_, (seq_t)(p + 1), true);
    }
    return rm;
}
