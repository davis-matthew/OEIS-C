/*
 * A171521  Numbers that are not the product of k-th composite number and k
 *          for any k >= 1.
 * https://oeis.org/A171521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   OEIS sequence A171521 – Numbers that are not the product of the
 *            k‑th composite number and k (k ≥ 1). The sequence is the
 *            complement of the product sequence p_k = k * c_k, where c_k is
 *            the k‑th composite number
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

static seq_t *excluded = NULL;   /* sorted excluded numbers (products) */
static size_t excl_len = 0;

/* Lazy initialization of excluded numbers up to a fixed bound.
/// The bound 20000 is sufficient for n up to about 10000.   */
static void init_excluded(void)
{
    if (excluded != NULL)
        return;                     /* already initialized */

    const idx_t BOUND = 20000;      /* upper bound for values needed   */
    const idx_t SIEVE_LIMIT = BOUND + 200;  /* a bit extra for composite list */

    /* Simple prime sieve to collect composite numbers. */
    bool *is_prime = (bool *)calloc(SIEVE_LIMIT + 1, sizeof(bool));
    for (idx_t i = 2; i <= SIEVE_LIMIT; ++i) is_prime[i] = true;
    for (idx_t i = 2; i * i <= SIEVE_LIMIT; ++i)
        if (is_prime[i])
            for (idx_t j = i * i; j <= SIEVE_LIMIT; j += i)
                is_prime[j] = false;

    /* Store composite numbers in an array. */
    size_t comp_cap = SIEVE_LIMIT - 1;      /* upper bound on count */
    uint32_t *composite = (uint32_t *)malloc(comp_cap * sizeof(uint32_t));
    size_t comp_len = 0;
    for (idx_t i = 4; i <= SIEVE_LIMIT; ++i)
        if (!is_prime[i])
            composite[comp_len++] = (uint32_t)i;

    free(is_prime);

    /* Compute excluded products k * c_k up to BOUND. */
    excl_len = 0;
    excluded = (seq_t *)malloc(comp_len * sizeof(seq_t));
    for (size_t k = 0; k < comp_len; ++k) {
        seq_t prod = (seq_t)(k + 1) * (seq_t)composite[k];
        if (prod > BOUND)
            break;          /* further products will be larger */
        excluded[excl_len++] = prod;
    }

    free(composite);
}

/* Main function computing the n-th term of the sequence.   */
seq_t a(idx_t n)
{
    init_excluded();

    idx_t val = 1;          /* candidate integer to test    */
    size_t cnt = 0;         /* how many accepted numbers so far */
    size_t j   = 0;         /* index in excluded array       */

    while (cnt < n) {
        seq_t val_seq = (seq_t)val;
        if (j < excl_len && excluded[j] == val_seq) {
            /* This integer is excluded; skip it. */
            ++j;
            ++val;
            continue;
        }

        /* Accept this integer. */
        ++cnt;
        if (cnt == n)
            return val_seq;

        ++val;
    }

    /* Never reached. */
    return (seq_t)0;
}
