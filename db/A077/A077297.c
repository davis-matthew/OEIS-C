/*
 * A077297  Partition the concatenation 1234567...of natural numbers into
 *          successive strings which are multiples of 4, all different and >
 *          4. (0 never taken as the most significant digit.)
 * https://oeis.org/A077297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a sequence obtained by partitioning the concatenation
 *            123456789101112… into successive strings which are multiples of
 *            4, all different and >4, with no leading zero for any part
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   digits_ensure_size_or_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function must work for all supported seq_t types and for
    arbitrary call order.  A minimal lazy global state is used to
    keep the already‑generated terms and the set of used values.  */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/digits_ensure_size_or_grow.h"

/*  Non‑intrusive hash table used only for seq_t values that are
    non‑zero (zero is never a valid term).  */
static uint64_t hash_seq_t(seq_t x)
{
    return (uint64_t)(x * 11400714819323198485ULL);
}

typedef struct {
    seq_t *table;
    size_t size;
    size_t count;
} HashSet;

static void hs_init(HashSet *hs)
{
    hs->size = 16;
    hs->count = 0;
    hs->table = (seq_t*)calloc(hs->size, sizeof(seq_t));
}

static void hs_free(HashSet *hs)
{
    free(hs->table);
    hs->table = NULL;
    hs->size = hs->count = 0;
}

static void hs_rehash(HashSet *hs)
{
    size_t new_size = hs->size << 1;
    seq_t *new_table = (seq_t*)calloc(new_size, sizeof(seq_t));
    for (size_t i = 0; i < hs->size; ++i) {
        seq_t key = hs->table[i];
        if (key) {
            uint64_t h = hash_seq_t(key);
            size_t idx = h & (new_size - 1);
            while (new_table[idx]) idx = (idx + 1) & (new_size - 1);
            new_table[idx] = key;
        }
    }
    free(hs->table);
    hs->table = new_table;
    hs->size = new_size;
}

static bool hs_contains(const HashSet *hs, seq_t key)
{
    if (!key) return false;
    uint64_t h = hash_seq_t(key);
    size_t idx = h & (hs->size - 1);
    while (hs->table[idx]) {
        if (hs->table[idx] == key) return true;
        idx = (idx + 1) & (hs->size - 1);
    }
    return false;
}

static void hs_insert(HashSet *hs, seq_t key)
{
    if (!key) return;
    if (hs_contains(hs, key)) return;
    if ((hs->count * 2) > hs->size) hs_rehash(hs);
    uint64_t h = hash_seq_t(key);
    size_t idx = h & (hs->size - 1);
    while (hs->table[idx]) idx = (idx + 1) & (hs->size - 1);
    hs->table[idx] = key;
    hs->count++;
}

static void digits_append_number(char **buf, size_t *len,
                                 size_t *cap, uint64_t n)
{
    char tmp[32];
    int l = snprintf(tmp, sizeof(tmp), "%llu",
                     (unsigned long long)n);
    digits_ensure_size_or_grow(buf, len, cap, *len + l);
    memcpy(*buf + *len, tmp, l);
    *len += l;
}

/*  Global persistent state.  */
typedef struct {
    seq_t *terms;          /* stored sequence terms   */
    size_t term_cnt;       /* number of terms computed */
    size_t term_cap;       /* capacity of terms array  */
    HashSet used;          /* set of values already used */
    char *digits;          /* concatenated digits of natural numbers */
    size_t digits_len;     /* current number of digits */
    size_t digits_cap;     /* capacity of digits buffer */
    uint64_t next_nat;     /* next natural number to append */
    size_t pos;            /* current position in digits for next split */
} SeqState;

static SeqState *get_state(void)
{
    static SeqState *state = NULL;
    if (!state) {
        state = (SeqState*)calloc(1, sizeof(SeqState));
        hs_init(&state->used);
    }
    return state;
}

/*  Ensure the digit buffer contains at least 'need' characters.  */
static void ensure_digits(SeqState *s, size_t need)
{
    while (s->digits_len < need) {
        digits_append_number(&s->digits, &s->digits_len,
                             &s->digits_cap, s->next_nat);
        s->next_nat++;
    }
}

/*  Ensure capacity of the terms array.  */
static void terms_ensure_size_or_grow(SeqState *s, size_t new_cnt)
{
    if (new_cnt <= s->term_cap) return;
    size_t new_cap = (s->term_cap > 0) ? s->term_cap : 16;
    while (new_cap < new_cnt) new_cap <<= 1;
    s->terms = (seq_t*)realloc(s->terms, new_cap * sizeof(seq_t));
    s->term_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined for offset 1 */
    SeqState *s = get_state();

    /*  Compute terms until we have at least n of them.  */
    while (s->term_cnt < (size_t)n) {
        size_t start = s->pos;
        seq_t val = 0;
        size_t len = 0;

        for (;;) {
            ++len;
            ensure_digits(s, start + len);          /* enough digits */
            int d = s->digits[start + len - 1] - '0';
            val = val * 10 + (seq_t)d;

            /*  Candidate is valid only if */
            /*  • val > 4  */
            /*  • multiple of 4  */
            /*  • not used before  */
            /*  • next character (if any) is not a leading 0  */
            if (val > (seq_t)4 && ((val & 3) == 0) &&
                !hs_contains(&s->used, val) &&
                (start + len == s->digits_len ||
                 s->digits[start + len] != '0')) {
                hs_insert(&s->used, val);
                terms_ensure_size_or_grow(s, s->term_cnt + 1);
                s->terms[s->term_cnt++] = val;
                s->pos = start + len;                /* next start */
                break;                               /* next term */
            }
            /*  otherwise extend len and continue  */
        }
    }
    return s->terms[n - 1];
}
