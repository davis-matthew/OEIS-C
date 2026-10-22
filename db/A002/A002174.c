/*
 * A002174  Values taken by reduced totient function psi(n).
 * https://oeis.org/A002174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   search. v is a term iff v = lcm of all lambda(p^k) that divide
 *            v, where lambda(2)=1, lambda(4)=2, lambda(2^k)=2^(k-2) for k>=3,
 *            and lambda(p^k)=p^(k-1)*(p-1) for odd primes p. Terms are listed
 *            in increasing order
 * Range:     u8 n <= 92, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a2174_contrib, a2174_gcd, a2174_isprime, a2174_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/a2174_contrib.h"
#include "../utils/a2174_gcd.h"
#include "../utils/a2174_isprime.h"
#include "../utils/a2174_ok.h"

static uint64_t *a2174_terms = NULL;
static idx_t a2174_cnt = 0, a2174_cap = 0;
static uint64_t a2174_next = 1;

seq_t a(idx_t n) {
    while (a2174_cnt < n) {
        if (a2174_ok(a2174_next)) {
            if (a2174_cnt == a2174_cap) {
                idx_t nc = a2174_cap ? a2174_cap * 2 : 1024;
                a2174_terms = (uint64_t *)realloc(a2174_terms, sizeof(uint64_t) * nc);
                a2174_cap = nc;
            }
            a2174_terms[a2174_cnt++] = a2174_next;
        }
        a2174_next++;
    }
    return (seq_t)a2174_terms[n - 1];
}
