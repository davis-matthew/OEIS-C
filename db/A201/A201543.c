/*
 * A201543  a(n) is the smallest positive integer not in {a(1), ..., a(n - 1)}
 *          such that the sum of the least significant digit of a(n) and the
 *          most significant digit of a(n - 1) is a prime, with a(1) = 1.
 * https://oeis.org/A201543
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(1)=1; for n>1, a(n) is the smallest unused positive integer
 *            such that LS(a(n)) + MS(a(n-1)) is a prime. LS(x) = least
 *            significant digit of x, MS(x) = most significant digit of x. The
 *            sequence is a derangement of the positive integers
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   ms_digit, prime_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/ms_digit.h"
#include "../utils/prime_sum.h"

static seq_t *seq = NULL;       /* array of computed terms */
static size_t seq_len = 0;      /* number of terms stored */
static size_t seq_cap = 0;      /* allocated capacity */

static bool *used = NULL;      /* flag array: used[i]==true if integer i already used */
static size_t used_cap = 0;    /* allocated capacity for used array */
static idx_t min_candidate = 1;/* smallest positive integer not yet used */

/* ensure that seq array can hold at least len_needed elements */
static void ensure_seq_capacity(size_t len_needed) {
    if (len_needed <= seq_cap) return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    if (new_cap < len_needed) new_cap = len_needed;
    seq_t *new_seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    if (!new_seq) abort();
    seq = new_seq;
    seq_cap = new_cap;
}

/* ensure that used array can hold indices up to idx_needed */
static void ensure_used_capacity(idx_t idx_needed) {
    if ((size_t)idx_needed < used_cap) return;
    size_t new_cap = used_cap ? used_cap * 2 : 16;
    while ((size_t)idx_needed >= new_cap) new_cap *= 2;
    bool *new_used = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!new_used) abort();
    /* initialise newly allocated part to false */
    memset(new_used + used_cap, 0, (new_cap - used_cap) * sizeof(bool));
    used = new_used;
    used_cap = new_cap;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;  /* offset 1 */
    /* compute terms up to n if necessary */
    for (idx_t i = seq_len + 1; i <= n; ++i) {
        ensure_seq_capacity((size_t)i);
        if (i == 1) {
            seq[0] = (seq_t)1;
            ensure_used_capacity(1);
            used[1] = true;
            min_candidate = 2;
        } else {
            seq_t prev = seq[i - 2];
            seq_t ms = ms_digit(prev);
            idx_t k = min_candidate;
            while (1) {
                ensure_used_capacity(k);
                if (!used[k]) {
                    int sum = (int)(k % 10) + (int)ms;
                    if (prime_sum(sum))
                        break;
                }
                ++k;
            }
            seq[i - 1] = (seq_t)k;
            used[k] = true;
            /* advance min_candidate if it was taken */
            if (k == min_candidate) {
                ++min_candidate;
                while ((size_t)min_candidate < used_cap && used[min_candidate])
                    ++min_candidate;
            }
        }
    }
    seq_len = n;
    return seq[n - 1];
}
