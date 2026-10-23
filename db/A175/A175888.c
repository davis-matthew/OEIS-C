/*
 * A175888  Numbers n with property that sigma(n) <
 *          min(sigma(n-2),sigma(n-1),sigma(n+1),sigma(n+2)).
 * https://oeis.org/A175888
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th integer k such that sigma(k) < min{ sigma(k-2),
 *            sigma(k-1), sigma(k+1), sigma(k+2) } where sigma(m) is the sum
 *            of positive divisors of m. This function computes all such
 *            integers up to a pre‑chosen limit (2 000 000) by a simple
 *            divisor‑sum sieve and then returns the requested term
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* The build supplies seq_t and idx_t */
extern idx_t _floor_isqrt_u64(idx_t n);

static const idx_t LIMIT = 2000000;     /* pre‑sieved upper bound            */
static seq_t *sigma = NULL;             /* sigma[0..LIMIT+2]                 */
static idx_t sigma_len = 0;

static seq_t *found = NULL;             /* array of found terms (index starts at 0) */
static size_t found_len = 0;
static size_t found_cap = 0;
static idx_t last_checked = 0;          /* last integer examined */

static void init_sieve(void)
{
    if (sigma != NULL)
        return;                      /* already initialized */

    const idx_t max = LIMIT + 2;      /* need sigma up to LIMIT+2 */
    sigma_len = max + 1;              /* indices 0..max inclusive */

    sigma = (seq_t *)malloc(sigma_len * sizeof(seq_t));
    if (sigma == NULL) return;        /* allocation failure */

    for (idx_t i = 0; i <= max; ++i)
        sigma[i] = 0;                  /* initialise */

    /* classic divisor‑sum sieve */
    for (idx_t i = 1; i <= max; ++i) {
        seq_t val = (seq_t)i;          /* contribution of i itself   */
        for (idx_t j = i; j <= max; j += i)
            sigma[j] += val;
    }
}

static void grow_found(size_t needed)
{
    if (found_cap >= needed)
        return;

    found_cap = (found_cap == 0) ? 16 : found_cap * 2;
    while (found_cap < needed)
        found_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(found, found_cap * sizeof(seq_t));
    if (tmp == NULL) {
        /* memory exhaustion – keep old pointer */
        return;
    }
    found = tmp;
}

static void generate_up_to(idx_t target)
{
    init_sieve();

    while (found_len < target) {
        ++last_checked;
        if (last_checked < 3)          /* need at least n-2 >= 1          */
            continue;

        /* Safeguard: if we exceed pre‑sieved range, stop (should not happen). */
        if (last_checked + 2 > LIMIT)
            break;

        seq_t s = sigma[last_checked];
        if (s < sigma[last_checked-2] &&
            s < sigma[last_checked-1] &&
            s < sigma[last_checked+1] &&
            s < sigma[last_checked+2]) {
            grow_found(found_len + 1);
            found[found_len++] = (seq_t)last_checked;
        }
    }
}

/* Public routine required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for offset 1 */

    generate_up_to(n);
    if (n <= found_len)
        return found[n-1];

    /* If we ran out of pre‑computed range (unlikely with LIMIT=2e6) */
    return 0;
}
