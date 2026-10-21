/*
 * A053008  Primes from products of split even-digit primes.
 * https://oeis.org/A053008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = primes obtained from the decimal digits of the product of
 *            an even‑digit prime's two halves. For each even‑digit prime p,
 *            list the two halves L and R (ignore p if R would start with 0).
 *            For the product = L*R, generate (in increasing order) all
 *            distinct prime permutations of its decimal digi...
 * Range:     u8 n <= 4, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   get_digits, next_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

#include "../utils/seq_realloc.h"   /* seq_realloc */
#include "../utils/get_digits.h"    /* get_digits */
#include "../utils/next_prime.h"    /* next_prime */

/* --------------------------------------------------------------------- */
/*   Helpers                                                     */
/* --------------------------------------------------------------------- */

/* Simple deterministic test for 64‑bit integers.  Works for the sizes
   encountered in this sequence. */
static bool is_prime_u64(uint64_t n) {
    if (n < 2) return false;
    if (n % 2 == 0) return n == 2;
    if (n % 3 == 0) return n == 3;
    for (uint64_t i = 5; i * i <= n; i += 6) {
        if (n % i == 0)   return false;
        if (n % (i + 2) == 0) return false;
    }
    return true;
}

/* Systematic generation of distinct permutations of a sorted digit list.
   'digits' must be of length 'len' and already sorted ascending.
   The callback 'add' is called with each permutation that forms a prime. */
typedef void (*prime_callback)(seq_t value, void *user_data);

static void permute_rec(int pos, int len, const uint8_t *digits,
                        bool *used, seq_t cur, prime_callback add,
                        void *user_data) {
    if (pos == len) {
        /* Check primality (cast to uint64_t; numbers are < 2^64) */
        if (is_prime_u64((uint64_t)cur))
            add(cur, user_data);
        return;
    }
    for (int i = 0; i < len; ++i) {
        if (used[i]) continue;
        if (pos == 0 && digits[i] == 0) continue;                /* no leading 0 */
        if (i > 0 && digits[i] == digits[i-1] && !used[i-1]) continue; /* skip dup */
        used[i] = true;
        permute_rec(pos+1, len, digits, used, cur*10 + digits[i], add, user_data);
        used[i] = false;
    }
}

/* Callback used to gather prime permutations into a buffer. */
struct prime_buf {
    seq_t *buf;
    size_t *cnt;
};

static void add_prime_callback(seq_t val, void *user_data) {
    struct prime_buf *p = (struct prime_buf *)user_data;
    if (*p->cnt < (p->buf != NULL ? 0 : 0)) return; /* dummy to silence warning */
    p->buf[(*p->cnt)++] = val;
}

/* Generate all prime permutations of the decimal digits of 'prod'.
   The primes are stored in 'buf' and the number of them in '*cnt'. */
static void generate_prime_permutations(seq_t prod,
                                        seq_t *buf, size_t *cnt) {
    uint8_t digits[32];
    size_t dlen = 0;
    get_digits(prod, digits, &dlen);

    /* Sort the digits to enable duplicate skipping. */
    for (size_t i = 1; i < dlen; ++i) {
        for (size_t j = i; j > 0 && digits[j] < digits[j-1]; --j) {
            uint8_t tmp = digits[j]; digits[j] = digits[j-1]; digits[j-1] = tmp;
        }
    }

    bool used[32] = {0};
    struct prime_buf data = { buf, cnt };
    permute_rec(0, (int)dlen, digits, used, 0, add_prime_callback, &data);
}

/* --------------------------------------------------------------------- */
/*   Main sequence function                                         */
/* --------------------------------------------------------------------- */

static int cmp_seq_t(const void *a, const void *b) {
    seq_t va = *(const seq_t *)a;
    seq_t vb = *(const seq_t *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

/* Append a single term to the global cache, expanding if necessary. */
static void append_term(seq_t val,
                        seq_t **cache, size_t *len, size_t *cap) {
    if (*len == *cap) {
        size_t ncap = (*cap == 0) ? 16 : (*cap << 1);
        *cache = seq_realloc(*cache, *cap, ncap);
        *cap   = ncap;
    }
    (*cache)[(*len)++] = val;
}

/* The sequence function. */
seq_t a(idx_t n) {
    /* Static cache for computed terms */
    static seq_t *terms = NULL;
    static size_t terms_len = 0;
    static size_t terms_cap = 0;

    /* Track the largest prime examined so far */
    static uint64_t last_prime_checked = 0;

    /* Generate terms until we have at least n of them */
    while (terms_len < n) {
        /* Find next prime greater than last_prime_checked */
        uint64_t p = next_prime(last_prime_checked);
        if (p == 0) break;           /* should not happen */
        last_prime_checked = p;

        /* Count decimal digits of p (fast) */
        size_t digits = 0;
        uint64_t tmp = p;
        do { ++digits; tmp /= 10; } while (tmp > 0);

        if (digits & 1) continue;    /* skip odd-digit primes */

        size_t half = digits >> 1;   /* digits/2 */

        /* Compute divisor = 10^half */
        seq_t divisor = 1;
        for (size_t i = 0; i < half; ++i) divisor *= 10;

        seq_t right = (seq_t)(p % divisor);
        /* right must have exactly 'half' digits (no leading zero) */
        if (half > 1) {
            if (right < divisor / 10) continue;
        } else { /* half == 1 */
            if (right == 0) continue;
        }

        seq_t left  = (seq_t)p / divisor;
        seq_t product = left * right;

        /* Generate prime permutations of product digits */
        /* Allocate a stack buffer for primes; 128 is enough for typical cases */
        seq_t prime_buf[128];
        size_t prime_cnt = 0;
        generate_prime_permutations(product, prime_buf, &prime_cnt);
        if (prime_cnt == 0) continue;

        /* Sort the primes found for this product */
        qsort(prime_buf, prime_cnt, sizeof(seq_t), cmp_seq_t);

        /* Append them to the global sequence */
        for (size_t i = 0; i < prime_cnt; ++i)
            append_term(prime_buf[i], &terms, &terms_len, &terms_cap);
    }

    /* Return the n-th term (1‑based indexing) */
    return terms[n - 1];
}
