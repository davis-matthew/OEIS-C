/*
 * A018503  Divisors of 530.
 * https://oeis.org/A018503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Divisors of 530 in ascending order (OEIS A018503) */

#include <stdlib.h>

/* Recursive generation of all divisors. */
static void
gen_divs_recursive(const idx_t *primes, const unsigned *exps, size_t pcnt,
                   size_t pos, seq_t current, seq_t *arr, size_t *idx)
{
    if (pos == pcnt) {
        arr[*idx] = current;
        (*idx)++;
        return;
    }
    seq_t mult = 1;
    for (unsigned e = 0; e <= exps[pos]; ++e) {
        gen_divs_recursive(primes, exps, pcnt, pos + 1,
                           current * mult, arr, idx);
        mult *= (seq_t)primes[pos];
    }
}

/* Simple insertion sort for a small array of seq_t. */
static void
sort_seq_t(seq_t *arr, size_t n)
{
    for (size_t i = 1; i < n; ++i) {
        seq_t key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

/* Cached list of divisors of 530. */
static seq_t *divisors = NULL;
static size_t ndivs    = 0;

/* Initialise the divisor cache on first use. */
static void
init_divisors(void)
{
    if (divisors != NULL) return;

    const idx_t target = 530ull;   /* the fixed number 530 */
    /* Factorise target by trial division. */
    idx_t n = target;
    idx_t primes[10];
    unsigned exps[10];
    size_t pcnt = 0;

    for (idx_t p = 2; p * p <= n; ++p) {
        if (n % p == 0) {
            unsigned e = 0;
            while (n % p == 0) {
                n /= p;
                e++;
            }
            primes[pcnt] = p;
            exps[pcnt]   = e;
            pcnt++;
        }
    }
    if (n > 1) {
        primes[pcnt] = n;
        exps[pcnt]   = 1;
        pcnt++;
    }

    /* Compute total number of divisors = ∏(exp+1). */
    size_t total = 1;
    for (size_t i = 0; i < pcnt; ++i) total *= (exps[i] + 1);

    divisors = (seq_t *)malloc(total * sizeof(seq_t));
    if (!divisors) return;          /* allocation failed – return zero later */

    size_t idx = 0;
    gen_divs_recursive(primes, exps, pcnt, 0, (seq_t)1, divisors, &idx);
    sort_seq_t(divisors, total);
    ndivs = total;
}

/* Main sequence function requested by the harness. */
seq_t a(idx_t n)
{
    init_divisors();
    if (n == 0 || n > ndivs) return 0;
    return divisors[n - 1];
}
