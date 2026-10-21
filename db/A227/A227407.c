/*
 * A227407  Prime numbers representing a date in "condensed European notation"
 *          DDMMYY.
 * https://oeis.org/A227407
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, fini, full, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 683, u32 n <= 3111, u64 n <= 3111,
 *            u128 n <= 3111, big any n
 * Helpers:   build_primes_upto, date_ok, seq_alloc
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
#include "../utils/build_primes_upto.h"
#include "../utils/seq_alloc.h"
#include "../utils/date_ok.h"

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
