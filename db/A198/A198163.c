/*
 * A198163  Primes from merging of 3 successive digits in decimal expansion of
 *          sqrt(2).
 * https://oeis.org/A198163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the n-th prime obtained from successive overlapping
 *            3‑digit windows of the decimal expansion of sqrt(2); 0‑leading
 *            numbers are discarded
 * Range:     u8 none, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define BASE 1000000000U        /* 10^9 for limb base */
#define BIGINT_MAX_LIMBS 30000  /* enough for ~270k decimal digits */

/* ---- Big integer ---------------------------------------------------- */
typedef struct {
    size_t n;                  /* number of limbs in use */
    uint32_t d[BIGINT_MAX_LIMBS];
} BigInt;

static inline void big_trim(BigInt *a) {
    while (a->n > 0 && a->d[a->n - 1] == 0)
        a->n--;
}

static inline void big_set_uint(BigInt *a, uint32_t val) {
    a->n = (val > 0) ? 1 : 0;
    if (val > 0) a->d[0] = val;
}

static inline void big_copy(const BigInt *src, BigInt *dst) {
    dst->n = src->n;
    memcpy(dst->d, src->d, src->n * sizeof(uint32_t));
}

static inline void big_mul_small(BigInt *a, uint32_t m) {
    if (m == 0 || a->n == 0) { a->n = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < a->n; ++i) {
        uint64_t prod = (uint64_t)a->d[i] * m + carry;
        a->d[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    if (carry) a->d[a->n++] = (uint32_t)carry;
    big_trim(a);
}

static inline void big_add_small(BigInt *a, uint32_t m) {
    if (m == 0) return;
    uint64_t carry = m;
    size_t i = 0;
    while (carry && i < a->n) {
        uint64_t sum = (uint64_t)a->d[i] + carry;
        a->d[i] = (uint32_t)(sum % BASE);
        carry = sum / BASE;
        ++i;
    }
    if (carry) a->d[a->n++] = (uint32_t)carry;
    big_trim(a);
}

static inline void big_mul_small_to(const BigInt *src, uint32_t m, BigInt *dst) {
    if (m == 0 || src->n == 0) { dst->n = 0; return; }
    uint64_t carry = 0;
    for (size_t i = 0; i < src->n; ++i) {
        uint64_t prod = (uint64_t)src->d[i] * m + carry;
        dst->d[i] = (uint32_t)(prod % BASE);
        carry = prod / BASE;
    }
    dst->n = src->n;
    if (carry) dst->d[dst->n++] = (uint32_t)carry;
    big_trim(dst);
}

static inline void big_add_small_to(BigInt *dst, uint32_t m) {
    big_add_small(dst, m);
}

static inline int big_cmp(const BigInt *a, const BigInt *b) {
    if (a->n != b->n) return (a->n < b->n) ? -1 : 1;
    for (size_t i = a->n; i-- > 0;) {
        if (a->d[i] != b->d[i]) return (a->d[i] < b->d[i]) ? -1 : 1;
    }
    return 0;
}

static inline void big_sub(BigInt *a, const BigInt *b) {
    assert(a->n >= b->n);
    uint64_t borrow = 0;
    for (size_t i = 0; i < a->n; ++i) {
        int64_t diff = (int64_t)a->d[i] - (i < b->n ? b->d[i] : 0) - borrow;
        if (diff < 0) {
            diff += BASE;
            borrow = 1;
        } else
            borrow = 0;
        a->d[i] = (uint32_t)diff;
    }
    assert(borrow == 0);
    big_trim(a);
}

/* ---- Sequence implementation ------------------------------------- */
static bool primes_table[1000];
static bool primes_inited = false;

static void init_primes(void) {
    if (primes_inited) return;
    memset(primes_table, 0, sizeof(primes_table));
    for (int i = 2; i < 1000; ++i) {
        bool isprime = true;
        for (int j = 2; j * j <= i; ++j)
            if (i % j == 0) { isprime = false; break; }
        primes_table[i] = isprime;
    }
    primes_inited = true;
}

/* Cached data */
static seq_t *seq_cache = NULL;
static size_t seq_cap = 0;
static size_t seq_len = 0;

static uint8_t *dig_cache = NULL;
static size_t dig_cap = 0;
static size_t dig_len = 0;

/* Big integers for sqrt(2) digit generation */
static BigInt p, r, c, d_val;

/* -------------------------------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;            /* sequence is 1-indexed */

    init_primes();

    /* ensure cache space for requested term */
    if (n > seq_cap) {
        size_t new_cap = (seq_cap == 0) ? 64 : seq_cap * 2;
        while (new_cap < n) new_cap *= 2;
        seq_t *newptr = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (!newptr) exit(EXIT_FAILURE);
        seq_cache = newptr;
        seq_cap = new_cap;
    }

    /* ensure decimal digits buffer */
    if (dig_len == 0) {
        dig_cap = 16384;
        dig_cache = (uint8_t*)malloc(dig_cap);
        if (!dig_cache) exit(EXIT_FAILURE);
    }

    /* initial sqrt(2) state - done only once */
    static bool init_done = false;
    if (!init_done) {
        big_set_uint(&p, 1);          /* integer part 1 */
        big_set_uint(&r, 1);          /* remainder 2 - 1^2 = 1 */
        init_done = true;
    }

    /* Compute until we have at least n terms */
    while (seq_len < n) {
        /* Step 1: bring down next pair of zeros */
        big_mul_small(&r, 100);

        /* Step 2: compute c = p * 20 */
        big_mul_small_to(&p, 20, &c);

        /* Step 3: find largest digit x (0..9) with (c+x)*x <= r */
        int chosen = 0;
        for (int x = 9; x >= 0; --x) {
            big_mul_small_to(&c, (uint32_t)x, &d_val);
            uint32_t add = (uint32_t)(x * x);
            if (add) big_add_small_to(&d_val, add);
            if (big_cmp(&d_val, &r) <= 0) {
                chosen = x;
                break;
            }
        }

        /* Append the digit */
        if (dig_len >= dig_cap) {
            dig_cap *= 2;
            uint8_t *tmp = (uint8_t*)realloc(dig_cache, dig_cap);
            if (!tmp) exit(EXIT_FAILURE);
            dig_cache = tmp;
        }
        dig_cache[dig_len++] = (uint8_t)chosen;

        /* Step 4: update p and r */
        big_mul_small(&p, 10);
        big_add_small(&p, chosen);

        big_sub(&r, &d_val);

        /* Sliding window check: only when at least 3 digits available */
        if (dig_len >= 3) {
            size_t i = dig_len - 3;
            if (dig_cache[i] != 0) {
                int val = dig_cache[i] * 100 + dig_cache[i + 1] * 10 + dig_cache[i + 2];
                if (primes_table[val]) {
                    seq_cache[seq_len++] = (seq_t)val;
                    if (seq_len >= seq_cap) {   /* safe guard */
                        size_t new_cap = seq_cap * 2;
                        seq_t *tmp = (seq_t*)realloc(seq_cache, new_cap * sizeof(seq_t));
                        if (!tmp) exit(EXIT_FAILURE);
                        seq_cache = tmp;
                        seq_cap = new_cap;
                    }
                }
            }
        }
    }
    return seq_cache[n - 1];
}
