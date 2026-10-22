/*
 * A007796  List of pairs of primes in reverse order, starting at 1.
 * https://oeis.org/A007796
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   list 1 followed by the primes: q(0)=1, q(1)=2, q(2)=3, q(3)=5,
 *            ...; a(n) = concatenation of q(2n-1) then q(2n-2) in decimal.
 *            The primes come from a sieve of Eratosthenes that grows as
 *            needed
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static uint64_t *q_list = 0;
static uint64_t q_count = 0;
static uint64_t sieve_limit = 0;

static void build_list(uint64_t need) {
    uint64_t limit, i, j;
    unsigned char *comp;
    if (q_count >= need) return;
    limit = sieve_limit ? sieve_limit : 1024;
    for (;;) {
        comp = (unsigned char *)malloc((size_t)(limit + 1));
        memset(comp, 0, (size_t)(limit + 1));
        for (i = 2; i * i <= limit; i++)
            if (!comp[i])
                for (j = i * i; j <= limit; j += i) comp[j] = 1;
        free(q_list);
        q_list = (uint64_t *)malloc((size_t)(limit + 2) * sizeof(uint64_t));
        q_count = 0;
        q_list[q_count++] = 1;
        for (i = 2; i <= limit; i++)
            if (!comp[i]) q_list[q_count++] = i;
        free(comp);
        sieve_limit = limit;
        if (q_count >= need) return;
        limit *= 2;
    }
}

seq_t a(idx_t n) {
    uint64_t big, small, t;
    seq_t mult, r;
    build_list((uint64_t)(2 * n));
    big = q_list[2 * n - 1];
    small = q_list[2 * n - 2];
    mult = 1;
    t = small;
    while (t > 0) {
        mult *= 10;
        t /= 10;
    }
    r = (seq_t)big;
    r = r * mult + (seq_t)small;
    return r;
}
