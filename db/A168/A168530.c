/*
 * A168530  Numbers that are the product of two distinct primes and they are
 *          sum of 3 consecutive products of two distinct primes.
 * https://oeis.org/A168530
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   build_primes, isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS sequence A168530: Numbers that are the product of two distinct primes and
 * are the sum of three consecutive semiprimes (products of two distinct primes).
 *
 * This implementation precomputes all semiprimes up to 3,000,000, then
 * scans consecutive triples to collect sums that are also distinct
 * semiprimes.  The resulting list is stored in a static array; the nth
 * term is returned in O(1) time after the first call.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_primes.h"
#include "../utils/isPrimeTrial.h"

/* ---------- Helper structures and functions ---------- */

static int cmp_seq_t(const void *a, const void *b)
{
    const seq_t *pa = (const seq_t *)a;
    const seq_t *pb = (const seq_t *)b;
    if (*pa < *pb) return -1;
    if (*pa > *pb) return 1;
    return 0;
}

static bool is_semiprime_distinct(seq_t n, const seq_t *primes, size_t pcount)
{
    if (n < 6) return false;        // smallest distinct semiprime is 2*3 = 6
    for (size_t i = 0; i < pcount; ++i) {
        seq_t p = primes[i];
        if (p * p > n) break;
        if (n % p == 0) {
            seq_t q = n / p;
            if (q == p) return false;          // not distinct
            return isPrimeTrial(q);            // q must be prime
        }
    }
    return false;   /* no factor found -> n is prime or 1 */
}

/* Dynamically growing array for seq_t values */
typedef struct {
    seq_t *data;
    size_t size;
    size_t capacity;
} dynarr_t;

static void dynarr_init(dynarr_t *a, size_t init_cap)
{
    a->capacity = init_cap;
    a->size = 0;
    a->data = (seq_t *)malloc(a->capacity * sizeof(seq_t));
}

static void dynarr_append(dynarr_t *a, seq_t value)
{
    if (a->size >= a->capacity) {
        a->capacity *= 2;
        a->data = (seq_t *)realloc(a->data, a->capacity * sizeof(seq_t));
    }
    a->data[a->size++] = value;
}

static void dynarr_free(dynarr_t *a)
{
    free(a->data);
    a->data = NULL;
    a->size = a->capacity = 0;
}

/* ---------- Main sequence generation ---------- */

static size_t max_needed = 10000;  /* generate at least this many terms */
static dynarr_t _ans;
static bool _initialized = false;

static void _generate_sequence(void)
{
    /* ----- 1. Generate primes up to 3,000,000 ----- */
    const idx_t PRIME_LIMIT = 3000000;
    idx_t *pr_arr = NULL;
    size_t pr_cnt = 0;
    build_primes(PRIME_LIMIT, &pr_arr, &pr_cnt);

    /* Copy primes into a dynarr of seq_t (static for later use) */
    dynarr_t primes = {NULL, 0, 0};
    dynarr_init(&primes, pr_cnt);
    for (size_t i = 0; i < pr_cnt; ++i)
        dynarr_append(&primes, (seq_t)pr_arr[i]);

    /* Free temporary array from build_primes */
    free(pr_arr);

    /* ----- 2. Generate all distinct semiprimes <= 3,000,000 ----- */
    const seq_t SEMI_LIMIT = 3000000;
    dynarr_t semis = {NULL, 0, 0};
    dynarr_init(&semis, 200000);   /* initial capacity */

    for (size_t i = 0; i < primes.size; ++i) {
        seq_t p = primes.data[i];
        for (size_t j = i + 1; j < primes.size; ++j) {
            seq_t q = primes.data[j];
            if (p > SEMI_LIMIT / q) break;  /* avoid overflow */
            dynarr_append(&semis, p * q);
        }
    }

    /* ----- 3. Sort the semiprime list ----- */
    qsort(semis.data, semis.size, sizeof(seq_t), cmp_seq_t);

    /* ----- 4. Scan consecutive triples and collect qualifying sums ----- */
    dynarr_init(&_ans, max_needed * 2);
    for (size_t i = 0; i + 2 < semis.size; ++i) {
        seq_t sum = semis.data[i] + semis.data[i + 1] + semis.data[i + 2];
        if (is_semiprime_distinct(sum, primes.data, primes.size)) {
            dynarr_append(&_ans, sum);
            if (_ans.size >= max_needed) break;  /* enough terms */
        }
    }

    /* ----- 5. Clean up temporary arrays ----- */
    dynarr_free(&semis);
    dynarr_free(&primes);
}

/* ---------- External function required by the test harness ---------- */

seq_t a(idx_t n)
{
    if (!_initialized) {
        _generate_sequence();
        _initialized = true;
    }
    /* n is 1-based */
    if (n == 0) return 0;   /* undefined, but guard */
    if ((idx_t)_ans.size < n) {
        /* This should not happen for the test cases (n <= 10000) */
        return 0;
    }
    return _ans.data[n - 1];
}
