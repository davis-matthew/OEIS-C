/*
 * A082886  a(n) = floor((prime(n+1)-prime(n))/log(prime(n))).
 * https://oeis.org/A082886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((prime(n+1)-prime(n))/log(prime(n)))
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/build_primes_upto.h"   /* build_primes_upto(limit, &primes, &cnt) */

seq_t a(idx_t n)
{
    if (n < 1)   /* sequence defined only for n ≥ 1 */
        return 0;

    /* static cache of primes: primes[0] == prime(1) */
    static uint64_t *primes = NULL;
    static size_t prime_cnt = 0;

    /* Need at least (n+1) primes: primes[n] == prime(n+1) */
    idx_t need = n + 1;
    if (prime_cnt < (size_t)need) {
        /* approximate upper bound for p_need */
        idx_t limit;
        if (need < 6) {
            limit = 15;                  /* enough for first few primes */
        } else {
            double dn = (double)need;
            limit = (idx_t)(dn * (log(dn) + log(log(dn))) + 3.0);
            if (limit < (idx_t)need * 5)   /* safety margin */
                limit = (idx_t)need * 5;
        }

        /* Rebuild until we have enough primes */
        while (1) {
            uint64_t *tmp = NULL;
            size_t tmpcnt = 0;
            build_primes_upto(limit, &tmp, &tmpcnt);
            if (tmpcnt >= (size_t)need) {
                free(primes);          /* free old table */
                primes = tmp;
                prime_cnt = tmpcnt;
                break;
            }
            free(tmp);
            limit *= 2;                /* increase limit and try again */
        }
    }

    /* prime(n) and prime(n+1) */
    uint64_t p_n    = primes[n - 1];
    uint64_t p_next = primes[n];
    uint64_t gap    = p_next - p_n;

    /* Compute floor((gap)/log(p_n)) using double precision */
    double ln = log((double)p_n);
    double val = (double)gap / ln;
    /* add tiny epsilon to avoid 0.999999 rounding */
    unsigned long long temp = (unsigned long long)(val + 1e-12);
    seq_t result = (seq_t)temp;
    return result;
}
