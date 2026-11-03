/*
 * A083126  Palindromes arising in A083125. a(n) = A083125(n)*A083125(n+1).
 * https://oeis.org/A083126
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = A083125(n) * A083125(n+1)
 * Range:     u8 n <= 5, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Validated: partial, pass, 47 terms through n = 47 (2026-11-03)
 * Helpers:   is_palindrome_base, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_palindrome_base.h"
#include "../utils/mem.h"

/* convert a seq_t value to a decimal string */
static void seq_to_str(seq_t v, char *buf, size_t *len)
{
    if (v == 0) {
        buf[0] = '0';
        *len = 1;
        return;
    }
    char tmp[64];
    int pos = 0;
    seq_t ten = (seq_t)10;
    while (v != 0) {
        tmp[pos++] = '0' + (int)(v % ten);
        v /= ten;
    }
    for (int i = 0; i < pos; ++i)
        buf[i] = tmp[pos - 1 - i];
    *len = pos;
}

#ifdef __cplusplus
#include <unordered_set>
#include <string>
#endif

/* Cache for the A083125 sequence (1‑based). */
static seq_t *A = NULL;          /* A[1..Alen] */
static size_t Acap = 0;
static idx_t Alen = 0;           /* number of elements stored */

/* Storage of used numbers.
   In C we use a boolean bitmap, in C++ an unordered_set of string keys. */
#ifndef __cplusplus
static bool *used = NULL;        /* bitmap of used numbers indexed by integer value */
static size_t used_cap = 0;
#else
static std::unordered_set<std::string> used_set;
#endif

/* Ensure bitmap can hold index idx (C only). */
#ifndef __cplusplus
static void ensure_used_capacity(size_t idx)
{
    if (idx + 1 < used_cap)
        return;
    size_t new_cap = used_cap ? used_cap : 128;
    while (new_cap <= idx)
        new_cap <<= 1;
    bool *tmp = (bool *)realloc(used, new_cap * sizeof(bool));
    if (!tmp)
        abort();
    for (size_t i = used_cap; i < new_cap; ++i)
        tmp[i] = false;
    used = tmp;
    used_cap = new_cap;
}
#endif

/* Ensure array A can store at least 'idx' elements (1‑based). */
static void ensure_A_capacity(idx_t idx)
{
    if ((size_t)idx <= Acap)
        return;
    size_t new_cap = Acap ? Acap : 16;
    while ((size_t)new_cap < (size_t)idx)
        new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, A, (new_cap + 1));
    if (!tmp)
        abort();
    A = tmp;
    Acap = new_cap;
}

/* Generate the next element of A083125 and append to A. */
static void generate_next_A(void)
{
    seq_t prev = A[Alen];          /* previous term */
    seq_t candidate = 1;

    for (;; ++candidate) {
#ifndef __cplusplus
        /* C implementation uses bitmap index. */
        size_t ci = (size_t)candidate;
        ensure_used_capacity(ci);
        if (used[ci])
            continue;
#else
        /* C++ implementation uses string set. */
        char buf[512];
        size_t len;
        seq_to_str(candidate, buf, &len);
        std::string key(buf, len);
        if (used_set.find(key) != used_set.end())
            continue;
#endif
        seq_t prod = prev * candidate;
        if (is_palindrome_base(prod, 10))
            break;
    }

    /* Store new term. */
    ensure_A_capacity(Alen + 1);
    ++Alen;
    A[Alen] = candidate;

    /* Mark as used. */
#ifndef __cplusplus
    used[(size_t)candidate] = true;
#else
    char buf[512];
    size_t len;
    seq_to_str(candidate, buf, &len);
    used_set.emplace(buf, buf + len);
#endif
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                  /* undefined for n<1, guard anyway */

    /* Initialise on first call. */
    if (!A) {
        Acap = 32;
        A = mem_new(seq_t, Acap + 1);
        if (!A)
            abort();
        A[1] = 1;
        Alen = 1;

#ifndef __cplusplus
        used_cap = 256;
        used = (bool *)calloc(used_cap, sizeof(bool));
        if (!used)
            abort();
        used[1] = true;
#else
        used_set.clear();
        used_set.reserve(256);
        used_set.emplace("1", "1" + 1); /* string "1" */
#endif
    }

    /* Ensure we have computed up to A[n+1]. */
    idx_t need = n + 1;
    while (Alen < need)
        generate_next_A();

    return A[n] * A[n + 1];
}
