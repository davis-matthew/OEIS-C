/*
 * A103507  a(n) = Least i > 1, such that 2n+1 = 2*A000040(i)+A000040(k) for
 *          some k>1, 0 if no such i exists.
 * https://oeis.org/A103507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least i > 1 such that 2n+1 = 2*A000040(i)+A000040(k) for
 *            some k>1; 0 if no such i exists
 * Range:     u8 n <= 108, u16 n <= 108, u32 n <= 108, u64 n <= 108,
 *            u128 n <= 108, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Return the a(n) sequence value for index n (n >= 1). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* out‑of‑range guard */

    seq_t target = (seq_t)n * 2 + 1;      /* the value 2n+1 */
    uint64_t max = (uint64_t)target;      /* integer upper bound for the sieve */

    /* Allocate and run a simple sieve of Eratosthenes up to max. */
    size_t sz = (size_t)(max + 1);
    bool *isComp = (bool*)malloc(sz * sizeof(bool));
    if (!isComp) return 0;                /* allocation failure – return 0 */
    for (size_t i = 0; i < sz; ++i) isComp[i] = false;
    for (uint64_t i = 2; i * i <= max; ++i) {
        if (!isComp[i]) {
            for (uint64_t j = i * i; j <= max; j += i)
                isComp[j] = true;
        }
    }

    /* Collect the primes in an array. */
    size_t cap = max / 10 + 10;           /* initial capacity (enough for small n) */
    uint64_t *primes = (uint64_t*)malloc(cap * sizeof(uint64_t));
    if (!primes) { free(isComp); return 0; }
    size_t cnt = 0;
    for (uint64_t i = 2; i <= max; ++i) {
        if (!isComp[i]) {
            if (cnt >= cap) {            /* grow the prime array if needed */
                cap *= 2;
                uint64_t *tmp = (uint64_t*)realloc(primes, cap * sizeof(uint64_t));
                if (!tmp) { free(primes); free(isComp); return 0; }
                primes = tmp;
            }
            primes[cnt++] = i;
        }
    }

    /* Search for the least i > 1 that satisfies the condition. */
    seq_t answer = 0;
    seq_t bound = target >= 3 ? ((target - 3) / 2) : 0;   /* required p_i <= bound */
    for (size_t idx = 2; idx <= cnt; ++idx) {            /* idx is the prime index i */
        seq_t p = primes[idx - 1];
        if (p > bound) break;
        seq_t t = target - (seq_t)2 * p;                 /* candidate prime value */
        if (t < 3) break;
        if (!isComp[(size_t)t]) {                       /* t is prime and k > 1 */
            answer = idx;
            break;
        }
    }

    free(primes);
    free(isComp);
    return answer;
}
