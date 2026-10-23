/*
 * A107012  Primes of the form x^2+xy+25y^2, with x and y nonnegative.
 * https://oeis.org/A107012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 1506, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Prime numbers of the form x^2 + x*y + 25*y^2   (x,y ≥ 0) */
/* OEIS A107012 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/is_prime.h"

/*--- internal storage -------------------------------------------*/

static seq_t *prime_list = NULL;    /* sorted list of found prime values   */
static size_t prime_cnt  = 0;       /* number of primes in prime_list     */
static size_t prime_cap  = 0;       /* capacity of prime_list             */

static size_t max_y = 0;            /* greatest y for which a node exists */
static seq_t *next_val = NULL;      /* next value for each y              */
static idx_t *next_x   = NULL;      /* current x for each y               */
static bool   *active  = NULL;      /* active flag per y                  */

static idx_t current_limit = 100;   /* upper bound for generated values    */

/*--- helper prototypes -------------------------------------------*/

static void ensure_prime_capacity (size_t needed);
static void add_prime            (seq_t value);
static void init_arrays          (size_t new_max_y);
static void advance_limit        (void);
static size_t find_min_y         (void);

/*--- function implementations ------------------------------------*/

static void ensure_prime_capacity (size_t needed)
{
    if (prime_cap < needed) {
        size_t new_cap = prime_cap ? prime_cap * 2 : 64;
        while (new_cap < needed) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(prime_list, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        prime_list = tmp;
        prime_cap  = new_cap;
    }
}

static void add_prime (seq_t value)
{
    ensure_prime_capacity(prime_cnt + 1);
    prime_list[prime_cnt++] = value;
}

/* allocate or extend the arrays for y = old_max_y+1 … new_max_y  */
static void init_arrays (size_t new_max_y)
{
    if (new_max_y <= max_y) return;
    size_t new_size = new_max_y + 1;   /* we ignore index 0          */
    next_val = (seq_t  *)realloc(next_val, new_size * sizeof(seq_t));
    next_x   = (idx_t  *)realloc(next_x,   new_size * sizeof(idx_t));
    active   = (bool   *)realloc(active,   new_size * sizeof(bool));

    for (size_t y = max_y + 1; y <= new_max_y; ++y) {
        next_x[y]   = 0;
        next_val[y] = (seq_t)25 * (seq_t)y * (seq_t)y;   /* x = 0  */
        active[y]   = true;
    }
    max_y = new_max_y;
}

/* increase the bound and introduce all new y's that become reachable */
static void advance_limit (void)
{
    current_limit <<= 1;
    size_t new_ymax = (size_t)sqrt((double)current_limit / 25.0);
    if (new_ymax < 1) new_ymax = 1;
    init_arrays(new_ymax);
}

/* find the y whose next value is the smallest among all active y's */
static size_t find_min_y (void)
{
    size_t best_y = 0;
    seq_t  best_v = 0;
    for (size_t y = 1; y <= max_y; ++y) {
        seq_t v = next_val[y];
        if (best_y == 0 || v < best_v) {
            best_y = y;
            best_v = v;
        }
    }
    return best_y;
}

/*--- the required sequence function ------------------------------*/

seq_t a (idx_t n)
{
    if (n == 0) return 0;            /* offset is 1 */

    /* make sure we have at least n primes */
    while (prime_cnt < (size_t)n) {
        /* initialise the first nodes if not done yet */
        if (max_y == 0) {
            size_t init_y = (size_t)sqrt((double)current_limit / 25.0);
            if (init_y < 1) init_y = 1;
            init_arrays(init_y);
        }

        size_t y = find_min_y();          /* smallest next value */
        seq_t  val = next_val[y];

        if (val > (seq_t)current_limit) { /* need a larger bound   */
            advance_limit();
            continue;
        }

        /* prime test – use the provided helper */
        if (is_prime((uint64_t)val))
            add_prime(val);

        /* advance in the sequence for this y */
        next_x[y] += 1;
        idx_t x = next_x[y];
        next_val[y] = (seq_t)x * x + (seq_t)x * (seq_t)y + (seq_t)25 * (seq_t)y * (seq_t)y;
    }

    return prime_list[n - 1];           /* a(1) is the first element   */
}
