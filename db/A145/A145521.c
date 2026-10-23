/*
 * A145521  Take the primes raised to prime exponents, arranged in numerical
 *          order (A053810). If A053810(n) = r(n)^q(n), where r(n) and q(n)
 *          are primes, then a(n) = q(n)^r(n).
 * https://oeis.org/A145521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n‑th value of the sequence obtained by swapping the
 *            base and exponent in the ordered list of prime powers p^q (p and
 *            q prime)
 * Range:     u8 none, u16 none, u32 n <= 19, u64 n <= 30, u128 n <= 31,
 *            big any n
 * Helpers:   big_pow, build_primes, seq_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_cmp.h"
#include "../utils/big_pow.h"
#include "../utils/build_primes.h"

/* Node of the min‐heap: current prime power p^q. */
typedef struct {
    idx_t   p;          /* base prime                                      */
    size_t  exp_idx;    /* index of current exponent prime in primes array */
    seq_t   val;        /* current value p^q                               */
} Node;

/* Restore heap property from position i downward. */
static void sift_down(Node *heap, size_t size, size_t i)
{
    while (1) {
        size_t left  = 2 * i + 1;
        size_t right = left + 1;
        size_t smallest = i;

        if (left < size &&
            seq_cmp((const void *)&heap[left].val,
                    (const void *)&heap[smallest].val) < 0)
            smallest = left;

        if (right < size &&
            seq_cmp((const void *)&heap[right].val,
                    (const void *)&heap[smallest].val) < 0)
            smallest = right;

        if (smallest == i)
            break;

        Node tmp = heap[i];
        heap[i] = heap[smallest];
        heap[smallest] = tmp;
        i = smallest;
    }
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* A generous prime limit to cover thousands of terms. */
    const idx_t PRIME_LIMIT = 2000000ULL;   /* ~1.5e5 primes */

    /* Static cache of primes. */
    static idx_t *primes = NULL;
    static size_t prime_cnt = 0;
    static bool primes_ready = false;

    if (!primes_ready) {
        build_primes(PRIME_LIMIT, &primes, &prime_cnt);
        primes_ready = true;
    }

    /* n is 1‑based.  Return 0 for the trivial negative case. */
    if (n == 0)
        return (seq_t)0;

    /* Allocate the min‑heap: one entry per base prime initially. */
    Node *heap = (Node *)malloc(prime_cnt * sizeof(Node));
    size_t heap_size = prime_cnt;

    /* Initialise heap with each prime raised to the first prime exponent 2. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        heap[i].p = primes[i];
        heap[i].exp_idx = 0;                     /* exponent = primes[0] = 2   */
        heap[i].val = big_pow((seq_t)primes[i],
                              (seq_t)primes[0]);  /* p^2 */
    }

    /* Build min‑heap. */
    for (size_t i = heap_size / 2; i-- > 0; )
        sift_down(heap, heap_size, i);

    seq_t result = (seq_t)0;

    /* Extract the smallest value target times. */
    for (size_t iter = 1; iter <= (size_t)n; ++iter) {
        Node *top = &heap[0];

        /* Compute a(n): swap base and exponent. */
        seq_t val = big_pow((seq_t)primes[top->exp_idx], (seq_t)top->p);
        result = val;

        /* If this is the requested term, finish. */
        if (iter == (size_t)n) {
            free(heap);
            return result;
        }

        /* Advance to the next exponent for the same base prime. */
        size_t next_idx = top->exp_idx + 1;
        if (next_idx < prime_cnt) {
            top->exp_idx = next_idx;
            top->val = big_pow((seq_t)top->p,
                               (seq_t)primes[next_idx]);
            sift_down(heap, heap_size, 0);
        } else {
            /* No more exponents for this base – remove it from the heap. */
            heap[0] = heap[heap_size - 1];
            --heap_size;
            sift_down(heap, heap_size, 0);
        }
    }

    /* Should never reach here. */
    free(heap);
    return result;
}
