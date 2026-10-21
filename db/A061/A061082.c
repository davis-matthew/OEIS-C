/*
 * A061082  a(n) = A053061(n)/n.
 * https://oeis.org/A061082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 10^floor(1 + 2*log10(n)) + n
 *
 * The exponent is floor(1 + 2*log10(n)) = {
 *   2*k+1  if n <= floor(10^k * sqrt(10))
 *   2*k+2  otherwise
 * }
 * where k = floor(log10(n)).  The thresholds floor(10^k * sqrt(10))
 * for 0 ≤ k ≤ 18 are pre‑computed for 64‑bit n.
 *
 * The routine uses only integer arithmetic on seq_t and the helper
 * pow10 function provided by utils/pow10.h.
 */
#include "../utils/pow10.h"

static const uint64_t THRESHOLDS[19] = {
    /* k = 0 .. 18 */
    3ULL,                 /* 10^0 * sqrt(10) */
    31ULL,                /* 10^1 * sqrt(10) */
    316ULL,               /* 10^2 * sqrt(10) */
    3162ULL,              /* 10^3 * sqrt(10) */
    31622ULL,             /* 10^4 * sqrt(10) */
    316227ULL,            /* 10^5 * sqrt(10) */
    3162277ULL,           /* 10^6 * sqrt(10) */
    31622776ULL,          /* 10^7 * sqrt(10) */
    316227766ULL,         /* 10^8 * sqrt(10) */
    3162277660ULL,        /* 10^9 * sqrt(10) */
    31622776601ULL,       /* 10^10 * sqrt(10) */
    316227766016ULL,      /* 10^11 * sqrt(10) */
    3162277660168ULL,     /* 10^12 * sqrt(10) */
    31622776601684ULL,    /* 10^13 * sqrt(10) */
    316227766016837ULL,   /* 10^14 * sqrt(10) */
    3162277660168379ULL,  /* 10^15 * sqrt(10) */
    31622776601683793ULL, /* 10^16 * sqrt(10) */
    316227766016837933ULL,/* 10^17 * sqrt(10) */
    3162277660168379331ULL/* 10^18 * sqrt(10) */
};

seq_t a(idx_t n) {
    /* Determine number of decimal digits of n (k = floor(log10(n))) */
    uint64_t tmp = (uint64_t)n;
    uint8_t d = 0;
    do {
        d++;
        tmp /= 10;
    } while (tmp != 0);
    uint8_t k = d - 1;                /* k = floor(log10(n)) */

    /* Choose exponent e */
    uint8_t e;
    if ((uint64_t)n <= THRESHOLDS[k]) {
        e = 2 * k + 1;
    } else {
        e = 2 * k + 2;
    }

    seq_t base = pow10((idx_t)e);     /* 10^e as seq_t */
    seq_t val  = (seq_t)n;            /* n as seq_t */
    return base + val;
}
