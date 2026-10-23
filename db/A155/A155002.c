/*
 * A155002  Triangle read by rows, A104762 * (A000129 * 0^(n-k)).
 * https://oeis.org/A155002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  eigen, nonn, easy, tabl
 * Formula:   a(n) = Fib(r - c + 1) * A215928(c) where (r, c) are the row and
 *            column indices obtained from the triangular numbering: * r =
 *            smallest integer with r*(r+1)/2 >= n * c = n - r*(r-1)/2
 *            Fib(1)=1, Fib(2)=1, ... A215928 is the Pell sequence prefaced
 *            with a 1: A215928(1)=1, A215928(2)=1, A215928(n)=Pell...
 * Range:     u8 n <= 44, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

static seq_t *fib_cache = NULL;
static size_t fib_cap = 0;
static idx_t fib_next = 3;  /* next index to compute (1‑based) */

static seq_t *pell_cache = NULL; /* Pell[0] = 0, Pell[1] = 1 */
static size_t pell_cap = 0;
static idx_t pell_next = 2;     /* next Pell index to compute */

/* Ensure factorial caches up to index 'need' (1‑based) */
static void ensure_fib(idx_t need) {
    if (need < 3) return; /* base cases already set */
    if (need < fib_next) return;
    /* grow cache */
    size_t new_cap = fib_cap ? fib_cap * 2 : 4;
    while (new_cap <= need) new_cap *= 2;
    seq_t *new_buf = (seq_t *)realloc(fib_cache, new_cap * sizeof(seq_t));
    if (!new_buf) { /* allocation failed: abort */
        exit(EXIT_FAILURE);
    }
    fib_cache = new_buf;
    fib_cap = new_cap;
    /* compute missing Fibonacci numbers */
    for (idx_t i = fib_next; i <= need; ++i) {
        fib_cache[i] = fib_cache[i-1] + fib_cache[i-2];
    }
    fib_next = need + 1;
}

static void ensure_pell(idx_t need) {
    if (need < 0) return;           /* Pell[0] already exists */
    if (need < pell_next) return;
    size_t new_cap = pell_cap ? pell_cap * 2 : 4;
    while (new_cap <= need + 1) new_cap *= 2; /* store up to need index */
    seq_t *new_buf = (seq_t *)realloc(pell_cache, new_cap * sizeof(seq_t));
    if (!new_buf) {
        exit(EXIT_FAILURE);
    }
    pell_cache = new_buf;
    pell_cap = new_cap;
    /* compute missing Pell numbers */
    for (idx_t i = pell_next; i <= need + 1; ++i) {
        pell_cache[i] = 2 * pell_cache[i-1] + pell_cache[i-2];
    }
    pell_next = need + 2; /* next available index */
}

/* Initialize caches on first call */
static void init_cache(void) {
    if (!fib_cache) {
        fib_cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!fib_cache) exit(EXIT_FAILURE);
        fib_cache[0] = 0;          /* unused */
        fib_cache[1] = 1;
        fib_cache[2] = 1;
        fib_cap = 4;
        fib_next = 3;
    }
    if (!pell_cache) {
        pell_cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!pell_cache) exit(EXIT_FAILURE);
        pell_cache[0] = 0;           /* Pell(0) */
        pell_cache[1] = 1;           /* Pell(1) */
        pell_cap = 4;
        pell_next = 2;
    }
}

/* A215928 value for index c (1‑based) */
static seq_t a215928(idx_t c) {
    if (c == 1 || c == 2) return (seq_t)1;
    /* Need Pell(c-1), with Pell[1] = 1, Pell[2] = 2... */
    ensure_pell(c - 1);
    return pell_cache[c - 1];
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n<1, but safe guard */
    init_cache();

    /* Find smallest r such that r*(r+1)/2 >= n */
    idx_t r = (idx_t)((sqrt((double)(8 * n + 1)) - 1) / 2);
    if (r * (r + 1) / 2 < n) r++;
    /* Compute triangular number of preceding rows */
    idx_t prev_tri = r * (r - 1) / 2;
    idx_t c = n - prev_tri;             /* column index (1‑based) */

    idx_t fib_idx = r - c + 1;           /* 1‑based Fibonacci index */
    ensure_fib(fib_idx);
    seq_t fib_val = fib_cache[fib_idx];

    seq_t pell_val = a215928(c);

    return fib_val * pell_val;
}
