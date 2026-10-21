/*
 * A064097  A quasi-logarithm defined inductively by a(1) = 0 and a(p) = 1 +
 *          a(p-1) if p is prime and a(n*m) = a(n) + a(m) if m,n > 1.
 * https://oeis.org/A064097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 0
   a(p)   = 1 + a(p-1)            if p is prime
   a(n*m) = a(n) + a(m)           if n,m > 1
   (This is equivalent to a(n) = Σ e_i * a(p_i) where n = Π p_i^e_i.) */

#include <stdlib.h>   /* for malloc, realloc, free */
#include <stddef.h>   /* for size_t */

/* The build supplies the integer types seq_t and idx_t; we use them directly. */

static seq_t *a_arr      = NULL;   /* a_arr[i] stores a(i) for i ≥ 1 */
static idx_t *spf_arr    = NULL;   /* spf_arr[i] stores the smallest prime factor of i */
static idx_t  a_size     = 0;      /* current allocated array size (max index + 1) */
static idx_t  a_computed = 0;      /* largest index for which a_arr[i] is valid */

/* Expand the arrays to accommodate indices up to n (inclusive). */
static void ensure_precompute(idx_t n)
{
    if (n <= a_computed) return;   /* already computed up to n */

    /* Grow arrays to at least n+1, rounding up to a power of two for simplicity. */
    idx_t new_size = n + 1;
    if (new_size < 16) new_size = 16;
    while (new_size <= a_size) new_size <<= 1;
    if (new_size == 0) new_size = 16;   /* initial allocation */

    /* Reallocate a_arr and spf_arr.  If they are NULL, malloc. */
    a_arr   = (seq_t *)realloc(a_arr,  new_size * sizeof(seq_t));
    spf_arr = (idx_t *)realloc(spf_arr, new_size * sizeof(idx_t));

    /* Initialise new entries in spf_arr to 0. */
    for (idx_t i = a_size; i < new_size; ++i) spf_arr[i] = 0;

    /* Sieve smallest prime factors up to new_size-1.  We recompute the whole range
       each time for simplicity; the cost remains tiny for n up to a few thousand. */
    if (new_size > 1) spf_arr[1] = 1;
    for (idx_t i = 2; i < new_size; ++i) spf_arr[i] = 0;
    for (idx_t i = 2; i < new_size; ++i) {
        if (spf_arr[i] == 0) {                 /* i is prime */
            spf_arr[i] = i;
            if (i <= new_size / i) {           /* avoid overflow of i*i */
                for (idx_t j = i * i; j < new_size; j += i)
                    if (spf_arr[j] == 0) spf_arr[j] = i;
            }
        }
    }

    /* Compute a(i) for all new indices.  We start from the already computed max. */
    if (a_computed == 0) {                     /* initialise a_arr[1] */
        a_arr[1] = (seq_t)0;
        a_computed = 1;
    }
    for (idx_t i = a_computed + 1; i < new_size; ++i) {
        if (i == 1) {                          /* safety, never reached here */
            a_arr[i] = (seq_t)0;
            continue;
        }
        if (spf_arr[i] == i) {                 /* i is prime */
            a_arr[i] = (seq_t)(1 + a_arr[i-1]);
        } else {                                /* composite: factorize using spf */
            seq_t sum = 0;
            idx_t tmp = i;
            while (tmp > 1) {
                idx_t p = spf_arr[tmp];
                int   e = 0;
                while (tmp % p == 0) {
                    tmp /= p;
                    ++e;
                }
                sum += (seq_t)e * a_arr[p];
            }
            a_arr[i] = sum;
        }
    }

    a_size = new_size;
    a_computed = new_size - 1;
}

/* Public function: return a(n) for any n ≥ 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;     /* undefined for 0, but we return 0 to be safe */
    ensure_precompute(n);
    return a_arr[n];
}
