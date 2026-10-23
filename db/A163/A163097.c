/*
 * A163097  Even numbers with an odd number of partitions.
 * https://oeis.org/A163097
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth even integer with an odd number of integer partitions (p(n) odd) */

#include <stdint.h>
#include <stdlib.h>

static uint8_t *parity = NULL;
static size_t   parity_cap = 0;
static idx_t    parity_last = 0;

static seq_t  *evens = NULL;
static size_t  evens_cap = 0;
static idx_t   evens_size = 0;

static void add_even(idx_t val)
{
    if (evens_size == evens_cap) {
        size_t new_cap = evens_cap ? evens_cap * 2 : 8;
        evens = (seq_t*)realloc(evens, new_cap * sizeof(seq_t));
        evens_cap = new_cap;
    }
    evens[evens_size++] = (seq_t)val;
}

static void init_parity(void)
{
    if (!parity) {
        parity_cap = 1;
        parity = (uint8_t*)malloc(parity_cap * sizeof(uint8_t));
        parity[0] = 1;            /* p(0) = 1 (odd) */
        parity_last = 0;
        /* 0 is even and has odd partitions */
        add_even(0);
    }
}

static void compute_parity_up_to(idx_t limit)
{
    if (limit <= parity_last) return;

    if ((size_t)(limit + 1) > parity_cap) {
        size_t new_cap = parity_cap ? parity_cap * 2 : 1;
        while ((size_t)(limit + 1) > new_cap) new_cap *= 2;
        parity = (uint8_t*)realloc(parity, new_cap * sizeof(uint8_t));
        parity_cap = new_cap;
    }

    for (idx_t i = parity_last + 1; i <= limit; ++i) {
        uint8_t val = 0;
        for (idx_t k = 1;; ++k) {
            idx_t pent1 = k * (3 * k - 1) / 2;
            if (pent1 > i) break;
            val ^= parity[(size_t)(i - pent1)];
            idx_t pent2 = k * (3 * k + 1) / 2;
            if (pent2 <= i) val ^= parity[(size_t)(i - pent2)];
        }
        parity[(size_t)i] = val;
        if ((i & 1ULL) == 0 && val) add_even(i);
    }
    parity_last = limit;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0U;   /* offset is 1, but guard anyway */

    init_parity();

    while (evens_size < n) {
        compute_parity_up_to(parity_last + 1);
    }
    return evens[n - 1];
}
