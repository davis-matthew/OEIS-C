/*
 * A141242  a(n) is the number of divisors of the n-th positive integer with a
 *          prime number of divisors. In other words, a(n) is the number of
 *          divisors of A009087(n).
 * https://oeis.org/A141242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000005(A009087(n)) i.e. a(n) is the number of divisors
 *            of the n‑th smallest integer that has a prime number of
 *            divisors. Those integers are of the form p^(q-1) where p and q
 *            are primes, and their divisor count is q. The function generates
 *            all p^(q-1) up to a dynamic limit, sorts them, and...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

typedef struct { uint64_t v; seq_t q; } Node;

static int cmp_nodes(const void *a, const void *b)
{
    const Node *A = (const Node *)a;
    const Node *B = (const Node *)b;
    if (A->v < B->v) return -1;
    if (A->v > B->v) return 1;
    return 0;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0; /* offset is 1 */

    /* lower bound for the search range; will be doubled if necessary */
    uint64_t limit = 200000ULL;
    for (;;)
    {
        /* primes for bases p */
        uint64_t *base_primes = NULL;
        size_t base_cnt = 0;
        build_primes_upto(limit, &base_primes, &base_cnt);
        if (!base_primes) return 0;

        /* upper bound on exponent e = q-1 => q <= log2(limit)+1 */
        uint64_t exp_max = 0;
        uint64_t t = limit;
        while (t > 1) { t >>= 1; exp_max++; }
        exp_max++;   /* +1 */

        uint64_t *exp_primes = NULL;
        size_t exp_cnt = 0;
        build_primes_upto(exp_max, &exp_primes, &exp_cnt);
        if (!exp_primes) { free(base_primes); return 0; }

        /* pre‑allocate buffer large enough to hold all generated nodes */
        size_t buffer_cap = base_cnt * exp_cnt;
        if ((buffer_cap == 0) || (buffer_cap > 2000000)) /* safety cap */
            buffer_cap = 2000000;
        Node *nodes = (Node *)malloc(buffer_cap * sizeof(Node));
        if (!nodes) { free(base_primes); free(exp_primes); return 0; }

        size_t nodes_len = 0;

        /* generate all p^(q-1) values that do not exceed limit */
        for (size_t pi = 0; pi < base_cnt; ++pi)
        {
            uint64_t p = base_primes[pi];
            for (size_t qi = 0; qi < exp_cnt; ++qi)
            {
                uint64_t q = exp_primes[qi];
                unsigned exp = (unsigned)(q - 1);
                uint64_t val = 1;
                bool overflow = false;
                for (unsigned k = 0; k < exp; ++k)
                {
                    if (val > limit / p) { overflow = true; break; }
                    val *= p;
                }
                if (overflow || val > limit)
                {
                    /* further larger q will only increase the value */
                    break;
                }
                if (nodes_len == buffer_cap)
                {
                    /* extend buffer if needed */
                    size_t new_cap = buffer_cap * 2;
                    Node *tmp = (Node *)realloc(nodes, new_cap * sizeof(Node));
                    if (!tmp) { /* on failure free and return */
                        free(nodes);
                        free(base_primes);
                        free(exp_primes);
                        return 0;
                    }
                    nodes = tmp;
                    buffer_cap = new_cap;
                }
                nodes[nodes_len].v = val;
                nodes[nodes_len].q = (seq_t)q;
                nodes_len++;
            }
        }

        free(base_primes);
        free(exp_primes);

        if (nodes_len >= (size_t)n)
        {
            qsort(nodes, nodes_len, sizeof(Node), cmp_nodes);
            seq_t result = nodes[n - 1].q;
            free(nodes);
            return result;
        }

        free(nodes);
        /* not enough values, enlarge search range and try again */
        limit *= 2ULL;
        if (limit == 0) limit = UINT64_MAX; /* guard against overflow */
    }
}
