/*
 * A075434  Numbers that are not the sum of two perfect powers.
 * https://oeis.org/A075434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: returns the nth integer that cannot be expressed as the sum of two
 * perfect powers (numbers m such that m = a^b with a>1, b>1; 1 is also
 * considered a perfect power). The function scans integers in ascending
 * order, checks whether each one can be written as p+q where p,q are in
 * the pre‑computed list of perfect powers, and returns the kth such
 * integer. */

/* Ensure the implementation is valid in both C11 and C++17. */
#ifndef MAX_PP_LIMIT
#  define MAX_PP_LIMIT 2000000ULL   /* upper bound for pre‑computed powers */
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef uint64_t pp_index_t;         /* for array indices */
static bool *is_pp   = NULL;         /* is_pp[x] == true if x is a perfect power */
static uint64_t *pp_arr = NULL;      /* array of all perfect powers up to MAX_PP_LIMIT */
static size_t pp_cnt = 0;            /* number of perfect powers stored */
static size_t pp_cap = 0;            /* capacity of pp_arr */
static bool pp_initialized = false;  /* flag to avoid re‑initialisation */

/* Append a perfect power to the list. */
static void pp_push(uint64_t val)
{
    if (pp_cnt == pp_cap) {
        pp_cap = pp_cap ? pp_cap * 2 : 128;
        pp_arr = (uint64_t *)realloc(pp_arr, pp_cap * sizeof(uint64_t));
        if (!pp_arr) abort();
    }
    pp_arr[pp_cnt++] = val;
}

/* Initialise the tables of perfect powers. */
static void pp_init(void)
{
    if (pp_initialized) return;

    is_pp = (bool *)calloc((size_t)MAX_PP_LIMIT + 1, sizeof(bool));
    if (!is_pp) abort();

    pp_cap = 128;
    pp_arr = (uint64_t *)malloc(pp_cap * sizeof(uint64_t));
    if (!pp_arr) abort();

    /* 1 is a perfect power (1^k). */
    is_pp[1] = true;
    pp_push(1);

    /* Generate a^b for a≥2, b≥2 up to MAX_PP_LIMIT. */
    for (uint64_t base = 2; base * base <= MAX_PP_LIMIT; ++base) {
        uint64_t val = base * base;            /* exponent b = 2 */
        while (val <= MAX_PP_LIMIT) {
            if (!is_pp[val]) is_pp[val] = true;
            pp_push(val);
            /* Prevent overflow: if val > MAX_PP_LIMIT/base stop. */
            if (val > MAX_PP_LIMIT / base) break;
            val *= base;                       /* next exponent */
        }
    }
    pp_initialized = true;
}

/* Return the n-th term of A075434 (1‑based index). */
seq_t a(idx_t n)
{
    pp_init();                     /* ensure tables are ready */

    size_t found = 0;              /* number of terms found so far */

    for (uint64_t x = 1; ; ++x) {
        bool representable = false;

        /* Scan all pre‑computed perfect powers. */
        for (size_t i = 0; i < pp_cnt; ++i) {
            uint64_t p = pp_arr[i];
            if (p > x) continue;                /* skip larger powers */

            uint64_t rem = x - p;
            if (rem > 0 && rem <= MAX_PP_LIMIT && is_pp[rem]) {
                representable = true;
                break;
            }
        }

        if (!representable) {
            ++found;
            if (found == (size_t)n)
                return (seq_t)x;                  /* cast to incrementally larger type if needed */
        }
    }

    /* Unreachable: the loop must return before reaching this point. */
    abort();
}
