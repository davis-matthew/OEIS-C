/*
 * A101420  Numbers which are the sum of two positive cubes and not divisible
 *          by 13.
 * https://oeis.org/A101420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th positive integer that is a sum of two positive
 *            cubes and not divisible by 13
 * Range:     u8 n <= 12, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   cube
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ------------------------------------------------------------ */
/* Required headers ------------------------------------------------ */
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>   /* for size_t */
#include "../utils/cube.h"

/* ------------------------------------------------------------ */
/* Helper: compute the cube of a seq_t value. */

/* ------------------------------------------------------------ */
/* Dynamically resizeable array of seq_t values. */
struct array {
    seq_t *data;     /* pointer to the array */
    size_t size;     /* number of elements stored */
    size_t capacity; /* allocated capacity */
};

static void array_push(struct array *a, seq_t v)
{
    if (a->size == a->capacity) {
        size_t newcap = a->capacity ? a->capacity * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(a->data, (newcap + 1) * sizeof(seq_t));
        if (!tmp) exit(1); /* memory exhausted */
        a->data = tmp;
        a->capacity = newcap;
    }
    a->data[a->size++] = v;
}

/* ------------------------------------------------------------ */
/* Cache structure holding state that persists between calls. */
static struct {
    idx_t       maxA;          /* largest cube root computed (>=1) */
    seq_t      *cubes;         /* cubes[1..maxA] (cubes[0] unused) */
    struct array results;      /* stored a(k) values, sorted */
    seq_t       lastCandidate; /* last integer that was examined */
} cache = { 0, NULL, {NULL, 0, 0}, (seq_t)0 };

/* Extend the cube array by one new element (for a = maxA + 1). */
static void extend_cubes(void)
{
    idx_t a = cache.maxA + 1;
    seq_t val = cube((seq_t)a);

    size_t newcap = cache.maxA ? cache.maxA * 2 : 16;
    seq_t *tmp = (seq_t *)realloc(cache.cubes, (newcap + 1) * sizeof(seq_t));
    if (!tmp) exit(1);
    cache.cubes = tmp;

    /* Ensure the array is large enough. */
    if (a <= newcap) {
        cache.cubes[a] = val;
    } else {
        /* Should not happen with the chosen growth strategy. */
        exit(1);
    }
    cache.maxA = a;
}

/* Ensure cubes[1..maxA] contain all cubes <= x. */
static void ensure_cubes_up_to(seq_t x)
{
    while (cache.maxA == 0 || cache.cubes[cache.maxA] <= x) {
        extend_cubes();
    }
}

/* Test whether x can be expressed as a^3 + b^3 with a,b >= 1. */
static bool is_sum_of_two_cubes(seq_t x)
{
    ensure_cubes_up_to(x);

    for (idx_t a = 1; a <= cache.maxA; ++a) {
        seq_t a3 = cache.cubes[a];
        if (a3 > x) break;
        seq_t rest = x - a3;

        /* Binary search for rest among cubes[1..a] */
        idx_t lo = 1, hi = a;
        while (lo <= hi) {
            idx_t mid = (lo + hi) >> 1;
            seq_t mid3 = cache.cubes[mid];
            if (mid3 == rest) return true;
            else if (mid3 < rest) lo = mid + 1;
            else hi = mid - 1;
        }
    }
    return false;
}

/* ------------------------------------------------------------ */
/* Public sequence routine. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;      /* undefined for n < 1 */

    /* Generate terms until we have at least n values. */
    while (cache.results.size < (size_t)n) {
        cache.lastCandidate += 1;          /* next integer to test */

        /* Skip multiples of 13. */
        if ((cache.lastCandidate % (seq_t)13) == 0) continue;

        /* Test sum-of-two-cube property. */
        if (is_sum_of_two_cubes(cache.lastCandidate)) {
            array_push(&cache.results, cache.lastCandidate);
        }
    }

    return cache.results.data[(size_t)(n - 1)];
}
