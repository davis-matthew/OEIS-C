/*
 * A106951  Primes of the form 3x^2+3xy+7y^2, with x and y nonnegative.
 * https://oeis.org/A106951
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   generate primes of the form 3x^2 + 3xy + 7y^2 by maintaining a
 *            min‑heap of streams (one stream per y) and extracting the
 *            smallest value repeatedly. Each stream increases its x
 *            coordinate and produces the next value of the quadratic for that
 *            y. Primes are collected in order of appearance
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   isprime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*======================================================================
  Includes
  ====================================================================*/
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isprime_big.h"

/*======================================================================
  Stream structure – one entry per y
  ====================================================================*/
typedef struct {
    seq_t  val; /* current value 3x^2 + 3xy + 7y^2 */
    idx_t  x;   /* current x for this y */
    idx_t  y;   /* fixed y for this stream */
} Stream;

/*======================================================================
  Min‑heap of streams
  ====================================================================*/
static Stream *heap      = NULL;
static size_t  heap_cap  = 0;
static size_t  heap_size = 0;

static void heap_ensure_cap(size_t need)
{
    if (heap_cap < need) {
        size_t new_cap = heap_cap ? heap_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        heap = (Stream *)realloc(heap, new_cap * sizeof(Stream));
        heap_cap = new_cap;
    }
}

static void heap_push(Stream s)
{
    heap_ensure_cap(heap_size + 1);
    size_t i = heap_size++;
    while (i > 0) {
        size_t p = (i - 1) >> 1;
        if (heap[p].val <= s.val) break;
        heap[i] = heap[p];
        i = p;
    }
    heap[i] = s;
}

static Stream heap_pop(void)
{
    Stream ret = heap[0];
    Stream last = heap[--heap_size];
    size_t i = 0;
    while (1) {
        size_t l = (i << 1) + 1;
        if (l >= heap_size) break;
        size_t r = l + 1;
        size_t s = (r < heap_size && heap[r].val < heap[l].val) ? r : l;
        if (last.val <= heap[s].val) break;
        heap[i] = heap[s];
        i = s;
    }
    heap[i] = last;
    return ret;
}

/*======================================================================
  Storage for primes
  ====================================================================*/
static seq_t *primes      = NULL;
static size_t primes_cap  = 0;
static size_t primes_size = 0;

static void add_prime(seq_t p)
{
    if (primes_size == primes_cap) {
        size_t new_cap = primes_cap ? primes_cap * 2 : 16;
        primes = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
        primes_cap = new_cap;
    }
    primes[primes_size++] = p;
}

/*======================================================================
  Heap initialization – one stream for each y up to Y_MAX
  ====================================================================*/
#define Y_MAX 20000   /* sufficient for n up to about 10000 */

static bool init_done = false;

static void init_sequence(void)
{
    if (init_done) return;
    init_done = true;

    for (idx_t y = 0; y <= Y_MAX; ++y) {
        Stream s;
        s.y = y;
        if (y == 0) {
            s.x  = 1; /* first prime from y = 0 is 3 */
            s.val = (seq_t)3;
        } else {
            s.x  = 0;
            s.val = 7 * ((seq_t)y * (seq_t)y);
        }
        heap_push(s);
    }
}

/*======================================================================
  Main function
  ====================================================================*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined for n < 1 */

    init_sequence();

    /* Generate primes until we have at least n of them */
    while (primes_size < (size_t)n) {
        Stream cur = heap_pop();

        /* Test current value for primality */
        if (isprime_big(cur.val)) {
            if (primes_size == 0 || cur.val != primes[primes_size - 1])
                add_prime(cur.val);
        }

        /* Advance the stream: compute next value */
        seq_t delta = 6 * ((seq_t)cur.x) + 3 + 3 * ((seq_t)cur.y);
        cur.val += delta;
        cur.x += 1;
        heap_push(cur);
    }

    return primes[n - 1];
}
