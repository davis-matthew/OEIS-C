/*
 * A105346  3-almost primes whose indices are 3-almost primes: a(n) =
 *          A014612(A014612(n)).
 * https://oeis.org/A105346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, less
 * Formula:   a(n) = A014612(A014612(n))
 * Range:     u8 n <= 12, u16 n <= 4306, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 3‑almost primes (numbers with exactly three prime factors,
   counting multiplicity).  This program builds an array of all such
   numbers up to a chosen bound and uses it to answer queries for
   a(n) = the 3‑almost prime at the position given by the 3‑almost
   prime at position n.  The array is rebuilt with a larger bound
   when more entries are required.  The implementation works for
   any seq_t that can hold the 3‑almost primes up to the required
   size (in the test harness seq_t may be an 8/16/32/64‑bit integer, or
   an arbitrary‑precision class).  No external tables are used.     */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* External definition required by the harness */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------ */
/*            Static storage for the 3‑almost‑prime list          */
/* ------------------------------------------------------------ */
static seq_t *three_list   = NULL;   /* 1–based: three_list[i] is the i‑th 3‑almost prime */
static size_t three_count  = 0;      /* entries in three_list, including dummy at 0 */
static uint64_t cur_limit  = 0;      /* current sieve upper bound */

/* ------------------------------------------------------------ */
/*             Build the list of 3‑almost primes up to limit      */
/* ------------------------------------------------------------ */
static void build_three_list(uint64_t limit)
{
    /* Free previously allocated memory if any */
    if (three_list != NULL) {
        free(three_list);
        three_list = NULL;
        three_count = 0;
    }

    /* Smallest prime factor array (1‑based) */
    uint32_t *lpf = (uint32_t *)calloc((size_t)(limit + 1), sizeof(uint32_t));
    if (lpf == NULL) exit(EXIT_FAILURE);

    /* Sieve: fill smallest prime factor for each number */
    for (uint64_t i = 2; i <= limit; ++i) {
        if (lpf[(size_t)i] == 0) {
            for (uint64_t j = i; j <= limit; j += i) {
                if (lpf[(size_t)j] == 0)
                    lpf[(size_t)j] = (uint32_t)i;
            }
        }
    }

    /* Compute Ω(n) (total number of prime factors, counted with multiplicity) */
    uint8_t *omega = (uint8_t *)calloc((size_t)(limit + 1), sizeof(uint8_t));
    if (omega == NULL) exit(EXIT_FAILURE);
    omega[1] = 0;
    for (uint32_t i = 2; i <= limit; ++i) {
        uint32_t p   = lpf[(size_t)i];
        uint32_t rest = (uint32_t)(i / p);
        omega[(size_t)i] = omega[(size_t)rest] + 1;
    }

    /* Count how many numbers satisfy Ω(n) == 3 */
    size_t cnt = 0;
    for (uint32_t i = 2; i <= limit; ++i)
        if (omega[(size_t)i] == 3)
            ++cnt;

    /* Allocate the 1‑based list (+1 for dummy at index 0) */
    three_list = (seq_t *)calloc(cnt + 1, sizeof(seq_t));
    if (three_list == NULL) exit(EXIT_FAILURE);
    three_list[0] = 0;          /* dummy */
    three_count  = cnt + 1;

    /* Fill the list */
    size_t pos = 1;
    for (uint32_t i = 2; i <= limit; ++i)
        if (omega[(size_t)i] == 3)
            three_list[pos++] = (seq_t)i;   /* cast to seq_t */

    /* Clean up temporary arrays */
    free(lpf);
    free(omega);
}

/* ------------------------------------------------------------ */
/*          Ensure the list has at least needed+1 entries           */
/* ------------------------------------------------------------ */
static void ensure_three_list(size_t needed)
{
    if (three_count > needed) return;  /* already fine */

    uint64_t new_limit = cur_limit ? cur_limit << 1 : 1000ULL;
    while (true) {
        build_three_list(new_limit);
        cur_limit = new_limit;
        if (three_count > needed) break;
        new_limit <<= 1;          /* double until enough */
    }
}

/* ------------------------------------------------------------ */
/*                         Public function                     */
/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n==0, return 0 */
        return (seq_t)0;

    /* First application of A014612: 3‑almost prime at position n */
    ensure_three_list((size_t)n);
    seq_t first = three_list[(size_t)n];

    /* Second application: 3‑almost prime at position first */
    idx_t idx2 = (idx_t)first;
    ensure_three_list((size_t)idx2);
    return three_list[(size_t)idx2];
}
