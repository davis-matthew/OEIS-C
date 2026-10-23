/*
 * A155893  Least positive integer such that a(n)! starts with n, both written
 *          in base 8.
 * https://oeis.org/A155893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 52, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A155893:
   Least positive integer such that a(n)! starts with n,
   both written in base 8. */

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Maximum number of octal digits that can appear in n (for n ≤ 10000). */
#define MAX_OCT_DIG 5
#define MAX_VAL (1 << 15)    /* 32768 ≥ 8^5 = 32768 */

/* Map[digits][value] = least m such that m! starts with 'value' (of length 'digits'). */
static int map[MAX_OCT_DIG + 1][MAX_VAL];

/* Conversion helpers: count octal digits of x (x ≥ 1). */
static int oct_digits(idx_t x)
{
    int d = 0;
    do {
        d++;
        x /= 8;
    } while (x);
    return d;
}

/* Pre‑computational state. */
static bool initialized = false;
static double logfact10 = 0.0;          /* sum of log10(k) for k = 1..m */
static idx_t current_m = 0;            /* last m processed */
static double log10_8 = 0.0;            /* log10(8) */
static int pow8[MAX_OCT_DIG + 1];      /* 8^i for i = 0..5 */

/*-----------------------------------------------------------------*/
/*   Find least m for which m! starts with n (in base 8)           */
/*-----------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* First call: initialise statically allocated arrays. */
    if (!initialized) {
        for (int d = 0; d <= MAX_OCT_DIG; d++)
            for (int v = 0; v < MAX_VAL; v++)
                map[d][v] = -1;
        log10_8 = log10(8.0);
        pow8[0] = 1;
        for (int i = 1; i <= MAX_OCT_DIG; i++)
            pow8[i] = pow8[i - 1] * 8;
        initialized = true;
    }

    if (n < 1)   /* the sequence is defined for n ≥ 1; caller never
                    gives smaller value, but guard anyway. */
        return 0;

    int need_digits = oct_digits(n);
    if (need_digits > MAX_OCT_DIG) {
        /* For n larger than 8^MAX_OCT_DIG-1 we do not support it here. */
        return 0;
    }

    /* If we already know the answer, return it. */
    if (map[need_digits][n] != -1)
        return (seq_t)map[need_digits][n];

    /* Incrementally process factorials until we get the wanted prefix. */
    while (map[need_digits][n] == -1) {
        current_m++;                                 /* next factorial */
        logfact10 += log10((double)current_m);       /* log10(current_m!) */

        double logfact8 = logfact10 / log10_8;        /* log base 8 */
        long long digits = (long long)floor(logfact8) + 1;

        double frac = logfact8 - floor(logfact8);

        /* For each possible prefix length, record the first m that
           yields that prefix. */
        for (int d = 1; d <= MAX_OCT_DIG; d++) {
            if (digits < d)            /* factorial too short for d digits */
                continue;

            double t = pow(8.0, frac + d - 1);            /* leading d digits as double */
            int val = (int)(t + 1e-12);                    /* truncate to integer */

            /* Numerical safety: clamp to legal range. */
            if (val >= pow8[d])
                val = pow8[d] - 1;
            if (val < pow8[d - 1])
                val = pow8[d - 1];

            if (val < MAX_VAL && map[d][val] == -1)
                map[d][val] = (int)current_m;
        }
    }

    return (seq_t)map[need_digits][n];
}
