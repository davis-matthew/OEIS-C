/*
 * A227407  Prime numbers representing a date in "condensed European notation"
 *          DDMMYY.
 * https://oeis.org/A227407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 477, u32 n <= 3111, u64 n <= 3111,
 *            u128 n <= 3111, big any n
 * Validated: full, pass, 3111 terms through n = 3111 (2026-11-02)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A227407: a(n) = nth prime that can be parsed as a valid date in DDMMYY (European) format.
   The sequence is finite; it contains all primes ≤ 311299 that represent a real
   date (Feb has only 28 days).  The function builds that list once on the
   first call and thereafter returns the nth element. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static bool date_ok(seq_t v)
{
    seq_t day   = v / (seq_t)10000;
    seq_t month = (v / (seq_t)100) % (seq_t)100;
    seq_t year  = v % (seq_t)100;

    if (month < (seq_t)1 || month > (seq_t)12) return false;

    int m = (int)month;
    int d = (int)day;
    int max_day;
    switch (m) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12: max_day = 31; break;
        case 4: case 6: case 9: case 11: max_day = 30; break;
        case 2: max_day = 28; break;           /* no leap years allowed */
        default: return false;
    }
    return d >= 1 && d <= max_day;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* sequence offset is 1 */

    static uint64_t *primes   = NULL;
    static size_t   primesCnt = 0;
    static seq_t   *datePrims = NULL;
    static size_t   dateCnt   = 0;
    static const seq_t MAXV   = (seq_t)311299;

    if (dateCnt == 0) {                       /* first invocation */
        /* generate all primes up to MAXV */
        build_primes_upto((idx_t)MAXV, &primes, &primesCnt);

        /* allocate storage for date‑primes; 4000 > 3111 */
        datePrims = seq_alloc(4000);
        dateCnt   = 0;

        for (size_t i = 0; i < primesCnt; ++i) {
            seq_t p = (seq_t)primes[i];
            if (p < (seq_t)10103) continue;     /* first date‑prime is 10103 */
            if (!date_ok(p)) continue;
            datePrims[dateCnt++] = p;
        }
    }

    if ((idx_t)(n - 1) < (idx_t)dateCnt)
        return datePrims[n - 1];                /* 1‑based indexing */

    return (seq_t)0;                            /* n beyond the finite sequence */
}
