/*
 * A092579  A sieve using the Fibonacci sequence over the integers >=2. Any
 *          multiple of a Fibonacci number, F(n)*m, such that F(n)>=2 and m>=2
 *          is excluded and what is left is included.
 * https://oeis.org/A092579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = nth positive integer ≥2 that is NOT a multiple of a
 *            Fibonacci number F (F≥2) with factor >=2. Equivalently, exclude
 *            n if ∃F∈{2,3,5,8,13,…} such that n%F==0 and n/F≥2. a(1)=2
 * Range:     u8 n <= 65, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Global storage for the sequence and Fibonacci numbers */
static seq_t *seq = NULL;          /* stored sequence values (only those that belong) */
static idx_t seq_len = 0;          /* number of values currently stored */
static idx_t seq_cap = 0;          /* allocated capacity for seq array */

static seq_t *fibs = NULL;         /* Fibonacci numbers (including 1,1) */
static idx_t fib_len = 0;          /* number of Fibonacci numbers stored */
static idx_t fib_cap = 0;          /* allocated capacity for fibs array */

/* Next integer to test (persistently across calls) */
static seq_t current = 2;

/* Grow the seq array if needed */
static void grow_seq(void) {
    if (seq_len < seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 8;
    seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    seq = tmp;
    seq_cap = new_cap;
}

/* Grow the fibs array if needed */
static void grow_fibs(void) {
    if (fib_len < fib_cap) return;
    size_t new_cap = fib_cap ? fib_cap * 2 : 8;
    seq_t *tmp = (seq_t *)realloc(fibs, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);
    fibs = tmp;
    fib_cap = new_cap;
}

/* Initialise Fibonacci numbers with 1,1 on first use */
static void init_fibs(void) {
    if (fib_len == 0) {
        grow_fibs();
        fibs[0] = (seq_t)1;
        fibs[1] = (seq_t)1;
        fib_len = 2;
    }
}

/* Generate inclusion sequence up to nth term */
seq_t a(idx_t n) {
    if (n == 0) {
        /* Sequence is 1‑based; return 0 for n==0 as a safe fallback */
        return (seq_t)0;
    }

    init_fibs();          /* ensure fibs is ready */

    /* Expand seq array to hold at least n elements */
    if (seq_cap < n) {
        seq_cap = n;
        seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
        if (!seq) exit(1);
    }

    /* Generate sequence until we have n terms stored */
    while (seq_len < n) {
        /* Generate Fibonacci numbers up to current/2 if needed */
        seq_t half = current / (seq_t)2;
        while (fib_len >= 2 && fibs[fib_len - 1] <= half) {
            grow_fibs();
            fibs[fib_len] = fibs[fib_len - 1] + fibs[fib_len - 2];
            ++fib_len;
        }

        /* Test whether current is excluded */
        int excluded = 0;
        for (idx_t i = 2; i < fib_len; ++i) { /* skip the 1,1 at indices 0,1 */
            seq_t f = fibs[i];
            if (f > half) break;
            if (current % f == (seq_t)0) {
                excluded = 1;
                break;
            }
        }

        if (!excluded) {
            /* Store the valid term */
            seq[seq_len] = current;
            ++seq_len;
        }

        ++current;   /* next candidate to test */
    }

    return seq[n - 1];
}
