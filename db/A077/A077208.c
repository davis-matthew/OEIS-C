/*
 * A077208  Rearrangement of squarefree numbers such that a(k) begins with the
 *          end of a(k-1).
 * https://oeis.org/A077208
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(1)=2; for n>1, a(n) is the smallest unused squarefree integer
 *            not ending in 0 whose first digit equals the last digit of
 *            a(n‑1)
 * Range:     u8 n <= 38, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Forward declaration used by the build system */
seq_t a(idx_t n);

/* --------------------------------------------------------------------- */
/* helpers for working with squarefree numbers                          */
/* --------------------------------------------------------------------- */
static bool is_squarefree(uint64_t x)
{
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % (p * p) == 0)
            return false;
    }
    return true;
}

static unsigned first_digit_uint64(uint64_t x)
{
    while (x >= 10)
        x /= 10;
    return (unsigned)x;
}

/* --------------------------------------------------------------------- */
/* static storage for the algorithm                                      */
/* --------------------------------------------------------------------- */
static bool init_done = false;

/* squarefree numbers list (ascending) */
static seq_t     *sf   = NULL;
static bool       *used = NULL;
static unsigned   *first_digit_vec = NULL;
static size_t      sf_size = 0;
static size_t      sf_cap  = 1 << 20;     /* 1,048,576 capacity */
static uint64_t    next_candidate = 0;    /* next integer to test */

/* pointer per decimal digit – next index in sf that might match that digit */
static size_t  *ptr_next = NULL;         /* array of 10 */

/* resulting sequence */
static seq_t   *seq   = NULL;
static size_t   seq_size = 0;
static size_t   seq_cap  = 1 << 20;      /* 1,048,576 capacity */

/* --------------------------------------------------------------------- */
/* dynamic enlargement of the squarefree list                            */
/* --------------------------------------------------------------------- */
static void extend_sf(size_t need)
{
    if (sf_size >= need)
        return;

    /* allocate or grow storage if necessary */
    if (sf_size == 0) {
        sf = (seq_t *)malloc(sf_cap * sizeof(seq_t));
        used = (bool *)malloc(sf_cap * sizeof(bool));
        first_digit_vec = (unsigned *)malloc(sf_cap * sizeof(unsigned));
    } else if (sf_size + (need - sf_size) > sf_cap) {
        size_t new_cap = (sf_cap * 3) / 2 + (need - sf_size) + 1024;
        sf = (seq_t *)realloc(sf, new_cap * sizeof(seq_t));
        used = (bool *)realloc(used, new_cap * sizeof(bool));
        first_digit_vec = (unsigned *)realloc(first_digit_vec,
                                              new_cap * sizeof(unsigned));
        sf_cap = new_cap;
    }

    uint64_t cand = next_candidate ? next_candidate : 2;
    while (sf_size < need) {
        if (cand % 10 == 0) {   /* skip numbers ending in 0 */
            ++cand;
            continue;
        }
        if (is_squarefree(cand)) {
            sf[sf_size] = (seq_t)cand;
            used[sf_size] = false;
            first_digit_vec[sf_size] = first_digit_uint64(cand);
            ++sf_size;
        }
        ++cand;
    }
    next_candidate = cand;
}

/* --------------------------------------------------------------------- */
/* initialise static data                                               */
/* --------------------------------------------------------------------- */
static void init(void)
{
    sf = (seq_t *)malloc(sf_cap * sizeof(seq_t));
    used = (bool *)malloc(sf_cap * sizeof(bool));
    first_digit_vec = (unsigned *)malloc(sf_cap * sizeof(unsigned));
    ptr_next = (size_t *)malloc(10 * sizeof(size_t));
    for (int d = 0; d < 10; ++d)
        ptr_next[d] = 0;

    seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
    init_done = true;
}

/* --------------------------------------------------------------------- */
/* main sequence function                                               */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (!init_done)
        init();

    if (n == 0)
        return 0;   /* not used, but defined for safety */

    size_t target = (size_t)n;
    if (seq_size < target) {
        /* ensure we have at least target squarefree numbers */
        extend_sf(target);

        /* generate sequence entries until we reach n */
        while (seq_size < target) {
            seq_t current;
            if (seq_size == 0) {
                current = (seq_t)2;
                /* find index of 2 in the squarefree list */
                size_t idx = 0;
                while (idx < sf_size && sf[idx] != (seq_t)2)
                    ++idx;
                if (idx < sf_size)
                    used[idx] = true;
            } else {
                seq_t last = seq[seq_size - 1];
                unsigned d = (unsigned)(last % 10);

                /* move ptr_next[d] until we find an unused number
                   whose first digit is d */
                while (ptr_next[d] < sf_size &&
                       (used[ptr_next[d]] ||
                        first_digit_vec[ptr_next[d]] != d)) 
                    ++ptr_next[d];

                /* if exhausted, generate more squarefree numbers */
                while (ptr_next[d] >= sf_size) {
                    extend_sf(sf_size + 1000);
                    while (ptr_next[d] < sf_size &&
                           (used[ptr_next[d]] ||
                            first_digit_vec[ptr_next[d]] != d)) 
                        ++ptr_next[d];
                }

                size_t idx = ptr_next[d];
                current = sf[idx];
                used[idx] = true;
            }

            seq[seq_size++] = current;
        }
    }

    return seq[n - 1];
}
