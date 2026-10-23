/*
 * A118570  Very lucky numbers : apply the sieve to obtain the lucky numbers
 *          (A000959) on the lucky numbers themselves.
 * https://oeis.org/A118570
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: generate lucky numbers, then apply the lucky sieve again
   to obtain the very lucky numbers (OEIS A118570).  The function
   seq_t a(idx_t n) returns the nth very lucky number (a(1)=1). */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/* Global storage for the intermediate and final sequences. */
static seq_t *lucky_list = NULL;      /* all lucky numbers up to a limit */
static size_t lucky_len  = 0;          /* number of lucky numbers */
static uint64_t lucky_limit = 2000ULL; /* current upper bound for lucky sieve */

static seq_t *very_list = NULL;       /* very lucky numbers after second sieve */
static size_t very_len  = 0;           /* number of very lucky numbers */

/* -------------------------------------------------------------
   Generate the list of lucky numbers up to `limit`.  The function
   allocates `lucky_list`, fills it with odd numbers, and then
   applies the lucky sieve in place.  `lucky_len` is set to the
   resulting length.
   ------------------------------------------------------------- */
static void generate_lucky(uint64_t limit)
{
    /* Allocate space for all odd numbers up to `limit`. */
    size_t init_cnt = (size_t)((limit + 1ULL) / 2ULL);
    lucky_list = (seq_t *)malloc(init_cnt * sizeof(seq_t));
    for (size_t i = 0; i < init_cnt; ++i) {
        lucky_list[i] = (seq_t)(2ULL * i + 1ULL);
    }

    size_t len = init_cnt;
    for (size_t i = 1; i < len; ++i) {
        seq_t step = lucky_list[i];
        if (step > (seq_t)len) break;        /* no more useful steps */
        size_t write = 0;
        /* Remove every `step`‑th element (counting from 1). */
        for (size_t pos = 1; pos <= len; ++pos) {
            if ((pos % (size_t)step) != 0U) {
                lucky_list[write++] = lucky_list[pos - 1];
            }
        }
        len = write;
    }
    lucky_len = len;
}

/* -------------------------------------------------------------
   Build the list of very lucky numbers by sieving the lucky list
   with its own elements as steps.  After this call, `very_list`
   points to an array of size `very_len`.
   ------------------------------------------------------------- */
static void compute_very_lucky(void)
{
    /* Ensure we have a lucky list ready. */
    if (lucky_list == NULL) return;  /* defensive */

    /* Start with a copy of the lucky list. */
    size_t len = lucky_len;
    very_list = (seq_t *)malloc(len * sizeof(seq_t));
    for (size_t i = 0; i < len; ++i) {
        very_list[i] = lucky_list[i];
    }

    /* Apply the lucky sieve again using the very lucky numbers. */
    size_t idx = 1;                     /* skip the first element '1' */
    while (idx < len) {
        seq_t step = very_list[idx];
        if (step > (seq_t)len) break;
        size_t write = 0;
        for (size_t pos = 1; pos <= len; ++pos) {
            if ((pos % (size_t)step) != 0U) {
                very_list[write++] = very_list[pos - 1];
            }
        }
        len = write;
        ++idx;
    }
    very_len = len;
}

/* -------------------------------------------------------------
   Ensure that at least `n` very lucky numbers have been computed.
   If not, grow the lucky limit, recompute everything, and repeat.
   ------------------------------------------------------------- */
static void ensure_very_len(idx_t n)
{
    while ((size_t)n > very_len) {
        /* Double the limit to grow the sieve. */
        lucky_limit *= 2ULL;
        if (lucky_limit == 0) lucky_limit = 1;   /* overflow guard */

        /* Free old storage. */
        free(lucky_list);
        lucky_list = NULL;
        lucky_len  = 0;

        free(very_list);
        very_list = NULL;
        very_len  = 0;

        /* Recompute with a larger limit. */
        generate_lucky(lucky_limit);
        compute_very_lucky();
    }
}

/* -------------------------------------------------------------
   Public function required by the build system.  It returns the
   nth very lucky number (with 1‑based indexing).  Undefined for
   n==0; the function returns 0 in that case.
   ------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* out of range */
    ensure_very_len(n);
    return very_list[(size_t)n - 1];
}
