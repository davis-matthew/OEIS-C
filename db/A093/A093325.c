/*
 * A093325  Numbers that are divisible by the sum of the factorials of their
 *          digits.
 * https://oeis.org/A093325
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the nth integer that is divisible by the sum of the
 *            factorials of its digits
 * Range:     u8 n <= 11, u16 n <= 251, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation precomputes the sequence lazily.  It keeps a static cache
   of those numbers already found, scanning integers one by one from the last
   checked value until the requested index is reached.  The algorithm runs in
   O(N) time where N is the largest number examined, which is well below the
   performance requirement for all practical inputs. */
#include <stddef.h>
#include <stdint.h>
/* seq_t and idx_t are supplied by the build environment */

static seq_t*   terms      = NULL;   /* cache of found terms */
static size_t   terms_cap  = 0;       /* cache capacity */
static size_t   terms_len  = 0;       /* number of terms currently cached */
static idx_t    last_checked = 1;     /* next integer to examine */

/* Precomputed factorial values 0! ... 9! */
static const seq_t fact[10] = {
    (seq_t)1U,   /* 0! */
    (seq_t)1U,   /* 1! */
    (seq_t)2U,   /* 2! */
    (seq_t)6U,   /* 3! */
    (seq_t)24U,  /* 4! */
    (seq_t)120U, /* 5! */
    (seq_t)720U, /* 6! */
    (seq_t)5040U,/* 7! */
    (seq_t)40320U,/* 8! */
    (seq_t)362880U /* 9! */
};

/* Ensure that the cache has room for at least 'need' elements. */
static void
ensure_terms_capacity(size_t need)
{
    if (terms_cap < need) {
        /* Double the capacity until it is enough. */
        size_t new_cap = terms_cap ? terms_cap * 2 : 16;
        if (new_cap < need) new_cap = need;
        seq_t* tmp = (seq_t*)realloc(terms, new_cap * sizeof(seq_t));
        if (!tmp) { /* out of memory; terminate */
            abort();
        }
        terms = tmp;
        terms_cap = new_cap;
    }
}

/* Compute the sum of factorials of the decimal digits of x. */
static seq_t
digit_factorial_sum(idx_t x)
{
    seq_t sum = 0;
    do {
        unsigned digit = (unsigned)(x % 10);
        sum += fact[digit];
        x /= 10;
    } while (x);
    return sum;
}

/* Trivial memoised sequence function. */
seq_t a(idx_t n)
{
    /* Ensure the cache contains at least 'n' terms. */
    while (terms_len < n) {
        /* Test last_checked for the property. */
        idx_t cur = last_checked;
        seq_t s = digit_factorial_sum(cur);
        if (s != 0) { /* s is always >0 but keep check for safety */
            seq_t num = (seq_t)cur;
            if (num % s == (seq_t)0) {
                /* Store the new term. */
                ensure_terms_capacity(terms_len + 1);
                terms[terms_len++] = num;
            }
        }
        last_checked = cur + 1;
    }
    /* Return a(n).  idx_t is assumed unsigned, so n-1 is safe. */
    return terms[n - 1];
}
